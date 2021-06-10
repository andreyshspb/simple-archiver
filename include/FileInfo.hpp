#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include <sys/stat.h>


class FileInfo {
public:
    FileInfo() = default;

    FileInfo(std::pair<dev_t, ino_t> parent,
             const std::string &name,
             const struct stat &state,
             const std::vector<char> &data);

    std::string getName() { return name_; }
    char *getData() { return data_.data(); }
    size_t getDataSize() { return data_.size(); }
    std::pair<dev_t, ino_t> getParent() { return parent_; }
    std::pair<dev_t, ino_t> getNode() {
        return {state_.st_dev, state_.st_ino};
    }
    mode_t getMode() { return state_.st_mode; }
    uid_t getUID() { return state_.st_uid; }
    gid_t getGID() { return state_.st_gid; }
    struct timespec getAccessTime() { return state_.st_atim; }
    struct timespec getModifiedTime() { return state_.st_mtim; }

    friend std::ifstream &operator>>(std::ifstream &in, FileInfo &info);

    friend std::ofstream &operator<<(std::ofstream &out, const FileInfo &info);

private:
    std::pair<dev_t, ino_t> parent_;
    std::string name_;
    struct stat state_;
    std::vector<char> data_;

};


std::ifstream &operator>>(std::ifstream &in, FileInfo &info);

std::ofstream &operator<<(std::ofstream &out, const FileInfo &info);
