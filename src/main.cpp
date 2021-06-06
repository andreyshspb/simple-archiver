#include "Archiver.hpp"

#include <iostream>


int main() {

    std::string sourcePath = "input";
    std::string placePath = "output";
    std::string archivePath = "archive.txt";

    Archiver archiver;
    archiver.create(sourcePath, archivePath);
    archiver.extract(placePath, archivePath);

    return 0;
}
