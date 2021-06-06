#include "Archiver.hpp"
#include "FileInfo.hpp"

#include <iostream>
#include <unordered_map>

#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>


void Archiver::create(const std::string &sourcePath, const std::string &archivePath) {
    archivePath_ = archivePath;
    archive_ = std::ofstream(archivePath, std::ios::binary);
    walk(sourcePath);
    archive_.close();
}


void Archiver::extract(const std::string &placePath, const std::string &archivePath) {
    std::unordered_map<size_t, std::string> nodeToPath;

    std::ifstream archive(archivePath, std::ios::binary);
    FileInfo info;
    while (archive >> info) {
        std::string path = placePath + '/' + nodeToPath[info.getParent()] + info.getName();
        std::cout << path << '\n';

        if (S_ISDIR(info.getMode())) {
            mkdir(path.c_str(), info.getMode());
            chown(path.c_str(), info.getUID(), info.getGID());
            nodeToPath[info.getNode()] = nodeToPath[info.getParent()] + info.getName() + '/';
        }

        if (S_ISREG(info.getMode()) && !nodeToPath.contains(info.getNode())) {
            int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, info.getMode());
            write(fd, reinterpret_cast<const void *>(info.getData()), info.getDataSize());
            fchown(fd, info.getUID(), info.getGID());
            close(fd);
            nodeToPath[info.getNode()] = nodeToPath[info.getParent()] + info.getName();
        } else if (S_ISREG(info.getMode()) && nodeToPath.contains(info.getNode())) {
            std::string from = placePath + '/' + nodeToPath[info.getNode()];
            link(from.c_str(), path.c_str());
            chown(path.c_str(), info.getUID(), info.getGID());
        }

    }
    archive.close();
}


void Archiver::walk(const std::string &path) {
    DIR *directory = opendir(path.c_str());

    struct stat dirInfo{};
    if (lstat(path.c_str(), &dirInfo) < 0) {
        throw std::runtime_error("something went wrong =(");
    }

    struct dirent *entry;
    while ((entry = readdir(directory)) != nullptr) {
        std::string name(entry->d_name);
        std::string currentFile = path + '/' + name;
        if (name == "." || name == "..") continue;
        if (pathsEqual(currentFile, archivePath_)) continue;

        struct stat info{};
        if (lstat(currentFile.c_str(), &info) < 0) {
            throw std::runtime_error("something went wrong =(");
        }

        std::vector<char> data;
        if (S_ISREG(info.st_mode) && !usedNode.contains(info.st_ino)) {
            data = takeData(currentFile);
            usedNode.insert(info.st_ino);
        }
        FileInfo fileInfo(dirInfo.st_ino, name, info, data);
        archive_ << fileInfo;

        if (S_ISDIR(info.st_mode)) {
            walk(currentFile);
        }
    }

    closedir(directory);
}


std::vector<char> Archiver::takeData(const std::string &path) {
    std::ifstream in(path);
    return std::vector<char>(std::istreambuf_iterator<char>(in),
                             std::istreambuf_iterator<char>());
}

std::string Archiver::pathHandler(const std::string &path) {
    if (path.starts_with("./")) {
        return path.substr(2, path.size());
    }
    if (path.starts_with("/")) {
        return path.substr(1, path.size());
    }
    return path;
}

bool Archiver::pathsEqual(const std::string &first, const std::string &second) {
    return pathHandler(first) == pathHandler(second);
}