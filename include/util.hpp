#pragma once

#include <iostream>
#include <vector>


namespace util {

    std::vector<char> takeDataFromFile(const std::string &path);
    std::string stripPath(const std::string &path);
    bool pathsEqual(const std::string &first, const std::string &second);

}  // namespace util

