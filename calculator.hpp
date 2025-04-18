// calculator.hpp
#ifndef __CALCULATOR_HPP
#define __CALCULATOR_HPP

#include <string>
#include <vector>

// --- Function expected by original tests ---
double parse_number(const std::string &expression);

// --- Functions for the main Lab 10 task ---

bool isValidDouble(const std::string &expression);

// Performs addition on two numbers represented as strings.
std::string addStrings(const std::string &num1, const std::string &num2);

// Helper function (useful for addStrings)
std::string normalizeNumberString(const std::string& num);

#endif // __CALCULATOR_HPP
