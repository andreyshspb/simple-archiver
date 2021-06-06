#pragma once


#include <iostream>


enum ProgramMode {
    CREATE,
    EXTRACT
};


struct DataCLI {
    ProgramMode mode;
    std::string placePath;
    std::string archivePath;
};


class ExceptionCLI : std::exception {
public:
    explicit ExceptionCLI(std::string message) : message_(std::move(message)) {}
    const char* what() const noexcept override { return message_.c_str(); }
private:
    std::string message_;
};


class ArchiverCLI {
public:
    ArchiverCLI() = default;
    DataCLI operator()(int argc, char *argv[]);
};
