cmake_minimum_required(VERSION 3.19)
project(archiver)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(archiver src/main.cpp src/Archiver.cpp include/Archiver.hpp
        include/FileInfo.hpp src/FileInfo.cpp
        include/CLI.hpp src/CLI.cpp
        include/util.hpp src/util.cpp)