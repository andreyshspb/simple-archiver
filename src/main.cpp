#include "Archiver.hpp"
#include "CLI.hpp"

#include <iostream>


int main(int argc, char *argv[]) {

    Archiver archiver;
    ArchiverCLI cli;

    try {
        DataCLI data = cli(argc, argv);
        switch (data.mode) {
            case CREATE:
                archiver.create(data.placePath, data.archivePath);
                break;
            case EXTRACT:
                archiver.extract(data.placePath, data.archivePath);
                break;
        }
    } catch (const ExceptionCLI &exception) {
        std::cout << exception.what() << std::endl;
    }

    return 0;
}
