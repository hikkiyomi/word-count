#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

void PrintUnknownOptionError(const std::string& option_name) {
    std::cerr << "Unknown option -- " << option_name << std::endl;
    std::cerr << "Try '.\\WordCount --help' to see more information" << std::endl;
    std::cerr << std::endl;
    std::cerr << "If you're putting a file which name starts with '-', try .\\filename" << std::endl;
    std::cerr << std::endl;
}

void PrintUnknownFileError(const std::string& file_path) {
    std::cerr << file_path << ": no such file or directory" << std::endl;
    std::cerr << std::endl;
}

void PrintNoFilesProvidedError() {
    std::cerr << "No files provided" << std::endl;
    std::cerr << "Try '.\\WordCount --help' for more information" << std::endl;
}

struct PrintOptions {
    bool print_lines;
    bool print_words;
    bool print_chars;
    bool print_bytes;

    PrintOptions()
        : print_lines(false)
        , print_words(false)
        , print_chars(false)
        , print_bytes(false)
    {}
};

void ToDefaultOptions(PrintOptions& options) {
    options.print_lines = true;
    options.print_words = true;
    options.print_bytes = true;
}

bool AnyOption(const PrintOptions& options) {
    return options.print_lines || options.print_words || options.print_chars || options.print_bytes;
}

bool UpdateOptions(PrintOptions& options, const std::string& arg) {
    if (arg.size() > 2 && arg[0] == '-' && arg[1] == '-') {
        std::string option_name = arg.substr(2);

        if (option_name == "lines") {
            options.print_lines = true;
        } else if (option_name == "words") {
            options.print_words = true;
        } else if (option_name == "chars") {
            options.print_chars = true;
        } else if (option_name == "bytes") {
            options.print_bytes = true;
        } else {
            PrintUnknownOptionError(option_name);
            exit(1);
        }

        return true;
    } else if (arg.size() > 1 && arg[0] == '-') {
        for (int j = 1; j < arg.size(); ++j) {
            char option_name = arg[j];

            if (option_name == 'l') {
                options.print_lines = true;
            } else if (option_name == 'w') {
                options.print_words = true;
            } else if (option_name == 'm') {
                options.print_chars = true;
            } else if (option_name == 'c') {
                options.print_bytes = true;
            } else {
                PrintUnknownOptionError(std::string(1, option_name));
                exit(1);
            }
        }

        return true;
    }
    
    return false;
}

void HelpCommand() {
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

void PrintResults(const std::vector<int>& results, const std::string& file_path) {
    for (auto result : results) {
        if (result < 0) continue;
        std::cout << result << " ";
    }

    std::cout << file_path << std::endl;
}

std::vector<int> AnalyzeFile(std::ifstream& file) {
    std::vector<int> count_results(4);
    const int kLines = 0, kWords = 1, kChars = 2, kBytes = 3;

    char current_char;
    int current_word_length = 0;

    while (file.get(current_char)) {
        if (current_char == '\n') {
            ++count_results[kLines];
        }

        if (!std::isspace(current_char)) {
            ++current_word_length;
        } else {
            if (current_word_length) ++count_results[kWords];
            current_word_length = 0;
        }

        ++count_results[kChars];
    }

    if (current_word_length) {
        ++count_results[kWords];
    }

    count_results[kBytes] = count_results[kChars]; // because we reached an agreement to use only ASCII symbols

    return count_results;
}

std::vector<int> ProcessFile(const std::string& file_path, const PrintOptions& options) {
    std::ifstream file(file_path, std::ios::binary);

    if (!file.is_open()) {
        PrintUnknownFileError(file_path);
        exit(1);
    }

    std::vector<int> count_results = AnalyzeFile(file);

    if (!options.print_lines) count_results[0] = -1;
    if (!options.print_words) count_results[1] = -1;
    if (!options.print_chars) count_results[2] = -1;
    if (!options.print_bytes) count_results[3] = -1;

    PrintResults(count_results, file_path);

    return count_results;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> files;
    PrintOptions options;

    for (int i = 1; i < argc; ++i) {
        std::string arg = std::string(argv[i]);

        if (arg == "--help") {
            HelpCommand();
            return 0;
        }

        if (UpdateOptions(options, arg)) continue;

        files.emplace_back(arg);
    }

    if (!AnyOption(options)) {
        ToDefaultOptions(options);
    }

    if (files.empty()) {
        PrintNoFilesProvidedError();
        return 1;
    }

    std::vector<int> overall_results(4);

    for (const std::string& file_path : files) {
        std::vector<int> file_results = ProcessFile(file_path, options);
        
        for (int i = 0; i < 4; ++i) {
            overall_results[i] += file_results[i];
        }
    }

    if (files.size() > 1) {
        PrintResults(overall_results, "total");
    }

    std::cout << std::endl;

    return 0;
}
