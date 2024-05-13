#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "SPA.h"

std::unique_ptr<std::string> readSource(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Unable to open file: " << filename << std::endl;
        exit(1);
    }

    std::unique_ptr<std::string> program = std::make_unique<std::string>(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()
    );

    file.close();
    return program;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <source_code>" << std::endl;
        return 1;
    }

    std::unique_ptr<std::string> source_code = readSource(argv[1]);

    SPA spa;
    spa.parseSimple(*source_code);

    std::string input;

    while (true) {
        std::cout << "Enter PQL query or enter 'quit' to exit: ";
        std::getline(std::cin, input);
        if (input == "quit") {
            break;
        } else {
            std::list<std::string> results;
            spa.evaluateQuery(input, results);

            for (auto& result : results) {
                std::cout << result << " ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}