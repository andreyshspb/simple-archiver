#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include <sys/stat.h>


class FileInfo {
public:
    FileInfo() = default;

    FileInfo(size_t parent,
             const std::string &name,
             const struct stat &state,
             const std::vector<char> &data);

    std::string getName() { return name_; }
    char *getData() { return data_.data(); }
    size_t getDataSize() { return data_.size(); }
    size_t getParent() { return parent_; }
    size_t getNode() { return state_.st_ino; }
    size_t getMode() { return state_.st_mode; }
    size_t getUID() { return state_.st_uid; }
    size_t getGID() { return state_.st_gid; }

    friend std::ifstream &operator>>(std::ifstream &in, FileInfo &info);

    friend std::ofstream &operator<<(std::ofstream &out, const FileInfo &info);

private:
    size_t parent_;
    std::string name_;
    struct stat state_;
    std::vector<char> data_;

};


std::ifstream &operator>>(std::ifstream &in, FileInfo &info);

std::ofstream &operator<<(std::ofstream &out, const FileInfo &info);
