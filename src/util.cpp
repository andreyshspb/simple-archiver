#include "util.hpp"

#include <fstream>


std::vector<char> util::takeDataFromFile(const std::string &path) {
    std::ifstream in(path);
    return std::vector<char>(std::istreambuf_iterator<char>(in),
                             std::istreambuf_iterator<char>());
}

std::string util::fillPath(const std::string &path) {
    if (path.ends_with("/")) {
        return path;
    }
    return path + "/";
}

