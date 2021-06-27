#pragma once

#include <iostream>
#include <vector>


namespace util {

    std::vector<char> takeDataFromFile(const std::string &path);
    std::string fillPath(const std::string &path);

}  // namespace util

