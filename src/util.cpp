#include "util.hpp"

#include <fstream>


std::vector<char> util::takeDataFromFile(const std::string &path) {
    std::ifstream in(path);
    return std::vector<char>(std::istreambuf_iterator<char>(in),
                             std::istreambuf_iterator<char>());
}

std::string util::stripPath(const std::string &path) {
    std::string result = path;
    if (result.starts_with("./")) {
        result = result.substr(result.size() - 2);
    }
    if (result.starts_with('/')) {
        result = result.substr(result.size() - 1);
    }
    if (result.ends_with('/')) {
        result = result.substr(0, result.size() - 1);
    }
    return result;
}

