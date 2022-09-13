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

int main(int argc, char* argv[]) {
    std::vector<std::string> args;

    for (int i = 1; i < argc; ++i) {
        std::string arg = std::string(argv[i]);

        if (arg == "--help") {
            helpCommand();
            return 0;
        }

        args.emplace_back(arg);
    }

    return 0;
}
