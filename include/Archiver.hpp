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
    std::string archivePath_;
    std::ofstream archive_;
    std::unordered_set<size_t> usedNode;

    void walk(const std::string &path);

    static std::vector<char> takeData(const std::string &path);
    static std::string pathHandler(const std::string &path);
    static bool pathsEqual(const std::string &first, const std::string &second);

};
