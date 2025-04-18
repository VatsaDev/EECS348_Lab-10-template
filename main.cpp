// main.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "calculator.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'" << std::endl;
        return 1;
    }

    std::string line;
    const std::string numberToAdd = "-123.456"; // The number to add

    while (std::getline(inputFile, line)) {
        // Trim leading/trailing whitespace (optional but good practice)
        line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

        if (line.empty()) {
            continue; // Skip empty lines
        }

        if (isValidDouble(line)) {
            std::string sum = addStrings(line, numberToAdd);
            std::cout << line << " + " << numberToAdd << " = " << sum << std::endl;
        } else {
            std::cout << "'" << line << "' is not a valid double number." << std::endl;
        }
    }

    inputFile.close();
    return 0;
}
