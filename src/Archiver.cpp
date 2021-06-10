#include "Archiver.hpp"
#include "FileInfo.hpp"
#include "util.hpp"

#include <iostream>
#include <unordered_map>

#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>


void Archiver::create(const std::string &inputPath, const std::string &archivePath) {
    archive_ = std::ofstream(archivePath, std::ios::binary);

    struct stat archiveInfo{};
    if (lstat(archivePath.c_str(), &archiveInfo) < 0) {
        throw std::runtime_error("something went wrong =(");
    }
    archiveNode_ = {archiveInfo.st_dev, archiveInfo.st_ino};

    walk(util::stripPath(inputPath));

    archive_.close();
}


void Archiver::extract(const std::string &outputPath, const std::string &archivePath) {
    std::unordered_map<ino_t , std::string> nodeToPath;
    std::vector<std::pair<std::string, std::pair<struct timespec, struct timespec>>> timeChanging;

    std::ifstream archive(archivePath, std::ios::binary);
    FileInfo info;
    while (archive >> info) {
        std::string path = util::stripPath(outputPath) + '/' + nodeToPath[info.getParent()] + info.getName();

        if (S_ISDIR(info.getMode())) {
            mkdir(path.c_str(), info.getMode());
            nodeToPath[info.getNode()] = nodeToPath[info.getParent()] + info.getName() + '/';
        } else if (S_ISREG(info.getMode()) && !nodeToPath.contains(info.getNode())) {
            int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, info.getMode());
            write(fd, reinterpret_cast<const void *>(info.getData()), info.getDataSize());
            close(fd);
            nodeToPath[info.getNode()] = nodeToPath[info.getParent()] + info.getName();
        } else if (nodeToPath.contains(info.getNode())) {
            std::string from = util::stripPath(outputPath) + '/' + nodeToPath[info.getNode()];
            link(from.c_str(), path.c_str());
        } else if (S_ISLNK(info.getMode())) {
            symlink(info.getData(), path.c_str());
            nodeToPath[info.getNode()] = nodeToPath[info.getParent()] + info.getName();
        } else if (S_ISFIFO(info.getMode())) {
            mkfifo(path.c_str(), info.getMode());
            nodeToPath[info.getNode()] = nodeToPath[info.getParent()] + info.getName();
        }

        if (S_ISLNK(info.getMode())) {
            lchown(path.c_str(), info.getUID(), info.getGID());
            lchmod(path.c_str(), info.getMode());
        } else {
            chown(path.c_str(), info.getUID(), info.getGID());
            chmod(path.c_str(), info.getMode());
        }

        timeChanging.push_back({path, {info.getAccessTime(), info.getModifiedTime()}});

    }
    archive.close();

    for (auto item : timeChanging) {
        struct timespec buffer[2] = { item.second.first, item.second.second };
        utimensat(AT_FDCWD, item.first.c_str(), buffer, AT_SYMLINK_NOFOLLOW);
    }
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

        struct stat info{};
        if (lstat(currentFile.c_str(), &info) < 0) {
            throw std::runtime_error("something went wrong =(");
        }
        std::pair<dev_t, ino_t> currentNode = {info.st_dev, info.st_ino};
        if (currentNode == archiveNode_) continue;

        std::vector<char> data;
        if (S_ISREG(info.st_mode) && !usedNode_.contains(info.st_ino)) {
            data = util::takeDataFromFile(currentFile);
        } else if (S_ISLNK(info.st_mode) && !usedNode_.contains(info.st_ino)) {
            size_t bufferSize = 256;
            char buffer[bufferSize];
            size_t pathSize = readlink(currentFile.c_str(), buffer, bufferSize);
            data.resize(pathSize);
            for (int i = 0; i < pathSize; i++) {
                data[i] = buffer[i];
            }
        }
        FileInfo fileInfo(dirInfo.st_ino, name, info, data);
        archive_ << fileInfo;

        usedNode_.insert(info.st_ino);

        if (S_ISDIR(info.st_mode)) {
            walk(currentFile);
        }
    }

    closedir(directory);
}
