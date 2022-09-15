#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

void throwUnknownOptionException(const std::string& optionName) {
    std::cerr << "Unknown option -- " << optionName << std::endl;
    std::cerr << "Try '.\\WordCount --help' to see more information" << std::endl;
    std::cerr << std::endl;
    std::cerr << "If you're putting a file which name starts with '-', try .\\filename" << std::endl;
    std::cerr << std::endl;
    exit(1);
}

void throwUnknownFileException(const std::string& filePath) {
    std::cerr << filePath << ": no such file or directory" << std::endl;
    std::cerr << std::endl;
    exit(1);
}

void helpCommand() {
    std::cout << "Usage: .\\WordCount [OPTION]... [FILES]..." << std::endl << std::endl;

    std::cout << "Options: " << std::endl;
    
    std::cout << std::left << std::setw(20) << "-l, --lines" << "print the line counts" << std::endl;
    std::cout << std::left << std::setw(20) << "-w, --words" << "print the word counts" << std::endl;
    std::cout << std::left << std::setw(20) << "-m, --chars" << "print the character counts" << std::endl;
    std::cout << std::left << std::setw(20) << "-c, --bytes" << "print the bytes counts" << std::endl;
    std::cout << std::left << std::setw(20) << "    --help" << "print help list" << std::endl;

    std::cout << std::endl;
    std::cout << "Output format (without options): lines words bytes filename" << std::endl;
    std::cout << "Output format (with options): lines words chars bytes filename" << std::endl << std::endl;
}

void printResults(const std::vector<int>& results, const std::string& filePath) {
    for (auto result : results) {
        if (result < 0) continue;
        std::cout << result << " ";
    }

    std::cout << filePath << std::endl;
}

std::vector<int> analyzeFile(std::ifstream& file) {
    std::vector<int> countResults(4);
    const int LINES = 0, WORDS = 1, CHARS = 2, BYTES = 3;

    char currentChar;
    int currentWordLength = 0;

    while (file.get(currentChar)) {
        if (currentChar == '\n') {
            ++countResults[LINES];
        }

        if (!std::isspace(currentChar)) {
            ++currentWordLength;
        }
        else {
            if (currentWordLength) ++countResults[WORDS];
            currentWordLength = 0;
        }

        ++countResults[CHARS];
    }

    if (currentWordLength) {
        ++countResults[WORDS];
    }

    countResults[BYTES] = countResults[CHARS];

    return countResults;
}

std::vector<int> processFile(const std::string& filePath,
                 const bool& printLines,
                 const bool& printWords,
                 const bool& printChars,
                 const bool& printBytes
) {
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open()) {
        throwUnknownFileException(filePath);
    }

    std::vector<int> countResults = analyzeFile(file);

    if (!printLines) countResults[0] = -1;
    if (!printWords) countResults[1] = -1;
    if (!printChars) countResults[2] = -1;
    if (!printBytes) countResults[3] = -1;

    file.close();

    printResults(countResults, filePath);

    return countResults;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> files;

    bool printLines = false;
    bool printWords = false;
    bool printChars = false;
    bool printBytes = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = std::string(argv[i]);

        if (arg == "--help") {
            helpCommand();
            return 0;
        }

        if ((int)arg.size() > 2 && arg[0] == '-' && arg[1] == '-') {
            std::string optionName = arg.substr(2);

            if (optionName == "lines") {
                printLines = true;
            }
            else if (optionName == "words") {
                printWords = true;
            }
            else if (optionName == "chars") {
                printChars = true;
            }
            else if (optionName == "bytes") {
                printBytes = true;
            }
            else {
                throwUnknownOptionException(optionName);
            }

            continue;
        }
        else if ((int)arg.size() > 1 && arg[0] == '-') {
            for (int j = 1; j < (int)arg.size(); ++j) {
                char optionName = arg[j];

                if (optionName == 'l') {
                    printLines = true;
                }
                else if (optionName == 'w') {
                    printWords = true;
                }
                else if (optionName == 'm') {
                    printChars = true;
                }
                else if (optionName == 'c') {
                    printBytes = true;
                }
                else {
                    throwUnknownOptionException(std::string(1, optionName));
                }
            }

            continue;
        }

        files.emplace_back(arg);
    }

    if (!printLines && !printWords && !printChars && !printBytes) {
        printLines = true;
        printWords = true;
        printBytes = true;
    }

    if (files.empty()) {
        std::cerr << "No files provided" << std::endl;
        std::cerr << "Try '.\\WordCount --help' for more information" << std::endl;
        return 1;
    }

    std::vector<int> overallResults(4);

    for (const std::string& filePath : files) {
        std::vector<int> fileResults = processFile(filePath, printLines, printWords, printBytes, printChars);
        
        for (int i = 0; i < 4; ++i) {
            overallResults[i] += fileResults[i];
        }
    }

    if ((int)files.size() > 1) {
        printResults(overallResults, "total");
    }

    std::cout << std::endl;

    return 0;
}
