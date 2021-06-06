#include "CLI.hpp"


#include <vector>


DataCLI ArchiverCLI::operator()(int argc, char *argv[]) {
    DataCLI result;

    bool createFlag = false;
    bool extractFlag = false;
    bool directoryFlag = false;

    std::vector<std::string> parameters;
    for (int i = 0; i < argc; i++) {
        parameters.emplace_back(argv[i]);
    }

    int cur = 1;
    while (cur < argc - 1) {
        std::string parameter = parameters[cur++];
        if (parameter == "-c" || parameter == "--create") {
            if (extractFlag) {
                throw ExceptionCLI("using both program mode");
            }
            if (createFlag) {
                throw ExceptionCLI("multiply using of -x flag");
            }
            result.mode = ProgramMode::CREATE;
            createFlag = true;
        } else if (parameter == "-x" || parameter == "--extract") {
            if (createFlag) {
                throw ExceptionCLI("using both program mode");
            }
            if (extractFlag) {
                throw ExceptionCLI("multiply using of -x flag");
            }
            result.mode = ProgramMode::EXTRACT;
            extractFlag = true;
        } else if (parameter == "-d" || parameter == "--directory") {
            if (directoryFlag) {
                throw ExceptionCLI("multiply using of -d flag");
            }
            if (cur == argc - 1) {
                throw ExceptionCLI("parameter for -d is not specified");
            }
            result.placePath = parameters[cur++];
            directoryFlag = true;
        } else {
            throw ExceptionCLI("incorrect flag -- " + parameter);
        }
    }

    result.archivePath = parameters.back();

    if (!createFlag && !extractFlag) {
        throw ExceptionCLI("mode of program is not specified");
    }

    return result;
}
