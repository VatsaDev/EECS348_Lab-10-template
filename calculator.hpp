#ifndef __CALCULATOR_HPP
#define __CALCULATOR_HPP

#include <string>
#include <vector>

// this function should expect that `expression` has already been
// validated and thus throw no exceptions
double parse_number(const std::string &expression);

bool isValidDouble(const std::string &expression);

std::string addStrings(const std::string &num1, const std::string &num2);

std::string normalizeNumberString(const std::string& num);

#endif // __CALCULATOR_HPP
