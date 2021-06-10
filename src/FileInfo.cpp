#include "FileInfo.hpp"

#include <fcntl.h>
#include <unistd.h>


FileInfo::FileInfo(std::pair<dev_t, ino_t> parent,
                   const std::string &name,
                   const struct stat &state,
                   const std::vector<char> &data) : parent_(parent),
                                                    name_(name),
                                                    state_(state),
                                                    data_(data) {}


std::ifstream &operator>>(std::ifstream &in, FileInfo &info) {
    in.read(reinterpret_cast<char *>(&info.parent_.first), sizeof(dev_t));
    if (in.eof()) return in;
    in.read(reinterpret_cast<char *>(&info.parent_.second), sizeof(ino_t));

    size_t nameLength;
    in.read(reinterpret_cast<char *>(&nameLength), sizeof(size_t));
    char *nameBuffer = new char[nameLength];
    in.read(nameBuffer, nameLength);
    info.name_.clear(); info.name_.resize(nameLength);
    for (int i = 0; i < nameLength; i++) {
        info.name_[i] = nameBuffer[i];
    }

    in.read(reinterpret_cast<char *>(&info.state_),
            sizeof(struct stat));

    size_t dataSize;
    in.read(reinterpret_cast<char *>(&dataSize), sizeof(size_t));
    char *dataBuffer = new char[dataSize];
    in.read(dataBuffer, dataSize);
    info.data_ = std::vector<char>(dataSize);
    for (int i = 0; i < dataSize; i++) {
        info.data_[i] = dataBuffer[i];
    }

    return in;
}

std::ofstream &operator<<(std::ofstream &out, const FileInfo &info) {
    out.write(reinterpret_cast<const char *>(&info.parent_.first), sizeof(dev_t));
    out.write(reinterpret_cast<const char *>(&info.parent_.second), sizeof(ino_t));

    size_t nameSize = info.name_.size();
    out.write(reinterpret_cast<const char *>(&nameSize), sizeof(size_t));
    out.write(info.name_.c_str(), nameSize);

    out.write(reinterpret_cast<const char *>(&info.state_),
              sizeof(struct stat));

    size_t dataSize = info.data_.size();
    out.write(reinterpret_cast<const char *>(&dataSize), sizeof(size_t));
    out.write(info.data_.data(), dataSize);

    return out;
}
