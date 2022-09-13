#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

void helpCommand() {
    std::cout << "Usage: .\\WordCount [OPTION]... [FILES]..." << std::endl << std::endl;

    std::cout << "Options: " << std::endl;
    
    std::cout << std::left << std::setw(20) << "-l, --lines" << "print the line counts" << std::endl;
    std::cout << std::left << std::setw(20) << "-c, --bytes" << "print the bytes counts" << std::endl;
    std::cout << std::left << std::setw(20) << "-w, --words" << "print the word counts" << std::endl;
    std::cout << std::left << std::setw(20) << "-m, --chars" << "print the character counts" << std::endl;
    std::cout << std::left << std::setw(20) << "    --help" << "print help list" << std::endl;

    std::cout << std::endl;
    std::cout << "Output format: lines words bytes filename" << std::endl << std::endl;
}

void throwUnknownOptionException(std::string optionName) {
    std::cout << "Unknown option -- " << optionName << std::endl;
    std::cout << "Try '.\\WordCount --help' to see more information" << std::endl;
    std::cout << std::endl;
    std::cout << "If you're putting a file which name starts with '-', try .\\filename" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> files;

    bool printLines = false;
    bool printBytes = false;
    bool printWords = false;
    bool printChars = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = std::string(argv[i]);

        if (arg == "--help") {
            helpCommand();
            return 0;
        }

        if (arg.size() > 2 && arg[0] == '-' && arg[1] == '-') {
            std::string optionName = arg.substr(2);

            if (optionName == "lines") {
                printLines = true;
            }
            else if (optionName == "bytes") {
                printBytes = true;
            }
            else if (optionName == "words") {
                printWords = true;
            }
            else if (optionName == "chars") {
                printChars = true;
            }
            else {
                throwUnknownOptionException(optionName);
                return 0;
            }

            continue;
        }
        else if (arg.size() > 1 && arg[0] == '-') {
            for (int j = 1; j < (int)arg.size(); ++j) {
                char optionName = arg[j];

                if (optionName == 'l') {
                    printLines = true;
                }
                else if (optionName == 'c') {
                    printBytes = true;
                }
                else if (optionName == 'w') {
                    printWords = true;
                }
                else if (optionName == 'm') {
                    printChars = true;
                }
                else {
                    throwUnknownOptionException(std::string(1, optionName));
                    return 0;
                }
            }

            continue;
        }

        files.emplace_back(arg);
    }

    return 0;
}
