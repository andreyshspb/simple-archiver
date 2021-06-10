#pragma once


#include <iosfwd>
#include <fstream>
#include <vector>
#include <unordered_set>


class Archiver {
public:
    Archiver() = default;

    void create(const std::string &inputPath, const std::string &archivePath);
    void extract(const std::string &outputPath, const std::string &archivePath);

private:
    std::pair<dev_t, ino_t> archiveNode_;
    std::ofstream archive_;
    std::unordered_set<ino_t> usedNode_;

    void walk(const std::string &path);

};
