#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

void printUnknownOptionError(const std::string& optionName) {
    std::cerr << "Unknown option -- " << optionName << std::endl;
    std::cerr << "Try '.\\WordCount --help' to see more information" << std::endl;
    std::cerr << std::endl;
    std::cerr << "If you're putting a file which name starts with '-', try .\\filename" << std::endl;
    std::cerr << std::endl;
}

void printUnknownFileError(const std::string& filePath) {
    std::cerr << filePath << ": no such file or directory" << std::endl;
    std::cerr << std::endl;
}

struct PrintOptions {
    bool printLines;
    bool printWords;
    bool printChars;
    bool printBytes;

    PrintOptions()
        : printLines(false)
        , printWords(false)
        , printChars(false)
        , printBytes(false)
    {}

    bool update(const std::string& arg) {
        if (arg.size() > 2 && arg[0] == '-' && arg[1] == '-') {
            std::string optionName = arg.substr(2);

            if (optionName == "lines") {
                printLines = true;
            } else if (optionName == "words") {
                printWords = true;
            } else if (optionName == "chars") {
                printChars = true;
            } else if (optionName == "bytes") {
                printBytes = true;
            } else {
                printUnknownOptionError(optionName);
                exit(1);
            }

            return true;
        } else if (arg.size() > 1 && arg[0] == '-') {
            for (int j = 1; j < arg.size(); ++j) {
                char optionName = arg[j];

                if (optionName == 'l') {
                    printLines = true;
                } else if (optionName == 'w') {
                    printWords = true;
                } else if (optionName == 'm') {
                    printChars = true;
                } else if (optionName == 'c') {
                    printBytes = true;
                } else {
                    printUnknownOptionError(std::string(1, optionName));
                    exit(1);
                }
            }

            return true;
        }
        
        return false;
    }

    bool any() const {
        return printLines || printWords || printChars || printBytes;
    }

    void to_default() {
        printLines = true;
        printWords = true;
        printBytes = true;
    }
};

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
        } else {
            if (currentWordLength) ++countResults[WORDS];
            currentWordLength = 0;
        }

        ++countResults[CHARS];
    }

    if (currentWordLength) {
        ++countResults[WORDS];
    }

    countResults[BYTES] = countResults[CHARS]; // because we reached an agreement to use only ASCII symbols

    return countResults;
}

std::vector<int> processFile(const std::string& filePath, const PrintOptions& options) {
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open()) {
        printUnknownFileError(filePath);
        exit(1);
    }

    std::vector<int> countResults = analyzeFile(file);

    if (!options.printLines) countResults[0] = -1;
    if (!options.printWords) countResults[1] = -1;
    if (!options.printChars) countResults[2] = -1;
    if (!options.printBytes) countResults[3] = -1;

    file.close();

    printResults(countResults, filePath);

    return countResults;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> files;
    PrintOptions options;

    for (int i = 1; i < argc; ++i) {
        std::string arg = std::string(argv[i]);

        if (arg == "--help") {
            helpCommand();
            return 0;
        }

        if (options.update(arg)) continue;

        files.emplace_back(arg);
    }

    if (!options.any()) {
        options.to_default();
    }

    if (files.empty()) {
        std::cerr << "No files provided" << std::endl;
        std::cerr << "Try '.\\WordCount --help' for more information" << std::endl;
        return 1;
    }

    std::vector<int> overallResults(4);

    for (const std::string& filePath : files) {
        std::vector<int> fileResults = processFile(filePath, options);
        
        for (int i = 0; i < 4; ++i) {
            overallResults[i] += fileResults[i];
        }
    }

    if (files.size() > 1) {
        printResults(overallResults, "total");
    }

    std::cout << std::endl;

    return 0;
}
