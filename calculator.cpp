// calculator.cpp
#include "calculator.hpp"
#include <string>
#include <vector>
#include <stdexcept> // Required for std::stod (if used in parse_number)
#include <iostream> // Debugging only
#include <algorithm> // For std::reverse, std::max

// --- Implementation of parse_number (for original tests) ---
double parse_number(const std::string &expression) {
    // Assuming expression is pre-validated as per the original comment.
    // Using std::stod for simplicity to pass the tests.
    // If this is disallowed, a manual string-to-double parsing logic
    // would be needed here, separate from isValidDouble and addStrings.
    try {
        return std::stod(expression);
    } catch (const std::invalid_argument& ia) {
        // Should not happen if expression is pre-validated, but handle defensively.
        std::cerr << "Error: Invalid argument passed to parse_number: " << expression << std::endl;
        return 0.0;
    } catch (const std::out_of_range& oor) {
        std::cerr << "Error: Number out of range in parse_number: " << expression << std::endl;
        // Return appropriate boundary value or handle as error
        if (expression.length() > 0 && expression[0] == '-') {
            return -HUGE_VAL; // Or std::numeric_limits<double>::lowest()
        } else {
            return HUGE_VAL; // Or std::numeric_limits<double>::max()
        }
    }
}


// --- Implementation of isValidDouble (for Lab 10 main task) ---
bool isValidDouble(const std::string &expression) {
    if (expression.empty()) {
        return false;
    }

    int i = 0;
    int n = expression.length();
    bool hasSign = false;
    bool hasDecimal = false;
    bool hasDigit = false;
    bool digitAfterDecimal = false;

    // Check for optional sign
    if (expression[0] == '+' || expression[0] == '-') {
        if (n == 1) return false; // Sign only is invalid
        hasSign = true;
        i = 1;
    }

    // Check for digits and decimal point
    bool expectingDigitAfterDecimal = false;
    for (; i < n; ++i) {
        if (std::isdigit(expression[i])) {
            hasDigit = true;
            if (hasDecimal) {
                digitAfterDecimal = true; // Saw a digit after the decimal
            }
             expectingDigitAfterDecimal = false; // Reset expectation if we see a digit
        } else if (expression[i] == '.') {
            if (hasDecimal) return false; // Multiple decimal points
            if (!hasDigit && i == (hasSign ? 1 : 0)) return false; // Decimal point cannot be the first character (after optional sign) e.g. ".5" or "+.5"
            hasDecimal = true;
            expectingDigitAfterDecimal = true; // Must see a digit next
        } else {
            return false; // Invalid character
        }
    }

    // Final checks
    if (!hasDigit) return false; // Must have at least one digit somewhere
    if (hasDecimal && !digitAfterDecimal) return false; // Decimal point exists but no digit followed it (e.g., "5.")

    return true;
}


// --- Implementation of addStrings and helpers (for Lab 10 main task) ---

// Helper: Compare absolute values of two normalized positive number strings
// Returns: -1 if num1 < num2, 0 if num1 == num2, 1 if num1 > num2
int compareAbsoluteStrings(std::string num1, std::string num2) {
    // Find decimal points
    size_t dec1 = num1.find('.');
    size_t dec2 = num2.find('.');
    std::string int1 = (dec1 == std::string::npos) ? num1 : num1.substr(0, dec1);
    std::string int2 = (dec2 == std::string::npos) ? num2 : num2.substr(0, dec2);
    std::string frac1 = (dec1 == std::string::npos) ? "" : num1.substr(dec1 + 1);
    std::string frac2 = (dec2 == std::string::npos) ? "" : num2.substr(dec2 + 1);

    // Compare integer parts by length first
     if (int1.length() > int2.length()) return 1;
     if (int1.length() < int2.length()) return -1;
     // Lengths are equal, compare lexicographically
     if (int1 > int2) return 1;
     if (int1 < int2) return -1;


    // Integer parts are equal, compare fractional parts (padded)
    size_t maxFracLen = std::max(frac1.length(), frac2.length());
    frac1.resize(maxFracLen, '0');
    frac2.resize(maxFracLen, '0');

    if (frac1 > frac2) return 1;
    if (frac1 < frac2) return -1;

    return 0; // Numbers are equal
}


// Helper: Add two positive number strings (already aligned)
std::string addAbsoluteStrings(std::string num1, std::string num2) {
    std::string result = "";
    int i = num1.length() - 1;
    int j = num2.length() - 1; // Should be same length after alignment
    int carry = 0;

    while (i >= 0 || j >= 0 || carry) {
         int digit1 = 0;
         int digit2 = 0;
         bool is_decimal_point = false;

         if (i >= 0) {
             if (num1[i] == '.') {
                 is_decimal_point = true;
             } else {
                 digit1 = num1[i] - '0';
             }
             i--;
         }
          // Since numbers are aligned, j will point to the same type (digit or '.')
         if (j >= 0) {
             if (num2[j] != '.') { // No need to check if num2[j] is '.', is_decimal_point handles it
                  digit2 = num2[j] - '0';
             }
             j--;
         }


        if (is_decimal_point) {
            result += '.';
        } else {
            int sum = digit1 + digit2 + carry;
            result += std::to_string(sum % 10);
            carry = sum / 10;
        }
    }
    std::reverse(result.begin(), result.end());
     // Handle case where result is empty (e.g., adding "0" and "0") or starts with "."
     if (result.empty()) return "0";
     if (result[0] == '.') result.insert(0, "0");
     return result;
}

// Helper: Subtract smaller positive string (num2) from larger positive string (num1) (already aligned)
std::string subtractAbsoluteStrings(std::string num1, std::string num2) {
    std::string result = "";
    int n1 = num1.length();
    int n2 = num2.length(); // Should be equal after alignment
    int borrow = 0;

    for (int i = n1 - 1; i >= 0; --i) {
        if (num1[i] == '.') {
            result += '.';
            continue;
        }

        // num2 might be shorter conceptually if padding failed, handle defensively
        // int digit1 = (i < n1) ? (num1[i] - '0') : 0; -> n1 check redundant due to loop condition
        int digit1 = num1[i] - '0';
        int digit2 = (i < n2 && num2[i] != '.') ? (num2[i] - '0') : 0;

        int diff = digit1 - digit2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result += std::to_string(diff);
    }

    std::reverse(result.begin(), result.end());
    // Normalization (especially removing leading zeros) will be done later
    // But handle if result is empty or starts with "."
    if (result.empty()) return "0";
    if (result[0] == '.') result.insert(0, "0");
    return result;
}

// Main addition function
std::string addStrings(const std::string &n1, const std::string &n2) {
    // Handle signs
    bool neg1 = n1[0] == '-';
    bool neg2 = n2[0] == '-';
    std::string absNum1_raw = neg1 ? n1.substr(1) : (n1[0] == '+' ? n1.substr(1) : n1);
    std::string absNum2_raw = neg2 ? n2.substr(1) : (n2[0] == '+' ? n2.substr(1) : n2);

    // Normalize inputs first to handle things like "007" or "5." before alignment
    std::string absNum1 = normalizeNumberString(absNum1_raw);
    std::string absNum2 = normalizeNumberString(absNum2_raw);

    // Pad numbers for alignment
    size_t dec1Pos = absNum1.find('.');
    size_t dec2Pos = absNum2.find('.');
    std::string intPart1 = (dec1Pos == std::string::npos) ? absNum1 : absNum1.substr(0, dec1Pos);
    std::string intPart2 = (dec2Pos == std::string::npos) ? absNum2 : absNum2.substr(0, dec2Pos);
    std::string fracPart1 = (dec1Pos == std::string::npos) ? "" : absNum1.substr(dec1Pos + 1);
    std::string fracPart2 = (dec2Pos == std::string::npos) ? "" : absNum2.substr(dec2Pos + 1);

    // Pad integer parts with leading zeros
    size_t maxIntLen = std::max(intPart1.length(), intPart2.length());
    while (intPart1.length() < maxIntLen) intPart1.insert(0, "0");
    while (intPart2.length() < maxIntLen) intPart2.insert(0, "0");

    // Pad fractional parts with trailing zeros
    size_t maxFracLen = std::max(fracPart1.length(), fracPart2.length());
    while (fracPart1.length() < maxFracLen) fracPart1.append("0");
    while (fracPart2.length() < maxFracLen) fracPart2.append("0");

    // Reconstruct aligned absolute numbers
    std::string alignedNum1 = intPart1;
    if (maxFracLen > 0) alignedNum1 += "." + fracPart1;
    // No need to add "." if maxFracLen is 0, even if original had it (like "5.")

    std::string alignedNum2 = intPart2;
    if (maxFracLen > 0) alignedNum2 += "." + fracPart2;


    std::string resultAbs = "";
    bool resultNeg = false;

     // Determine effective signs after potential normalization (e.g., "-0" becomes non-negative)
     if (absNum1 == "0") neg1 = false;
     if (absNum2 == "0") neg2 = false;


    if (neg1 == neg2) { // Both positive or both negative
        resultAbs = addAbsoluteStrings(alignedNum1, alignedNum2);
        resultNeg = neg1; // Result has the same sign (unless result is "0")
    } else { // Different signs - perform subtraction
        int cmp = compareAbsoluteStrings(absNum1, absNum2); // Compare pre-aligned normalized values
        if (cmp == 0) {
            resultAbs = "0"; // Numbers are equal magnitude, opposite sign
            resultNeg = false;
        } else if (cmp > 0) { // abs(num1) > abs(num2)
            resultAbs = subtractAbsoluteStrings(alignedNum1, alignedNum2);
            resultNeg = neg1; // Result has the sign of the originally larger magnitude number (num1)
        } else { // abs(num1) < abs(num2)
            resultAbs = subtractAbsoluteStrings(alignedNum2, alignedNum1);
            resultNeg = neg2; // Result has the sign of the originally larger magnitude number (num2)
        }
    }

    // Normalize the final result
    std::string finalResult = normalizeNumberString(resultAbs);

    // Avoid "-0"
    if (finalResult == "0") {
        return "0";
    }

    if (resultNeg && finalResult != "0") {
        return "-" + finalResult;
    } else {
        return finalResult;
    }
}


// Helper function to normalize string representation of a number
std::string normalizeNumberString(const std::string& num_str) {
     if (num_str.empty() || num_str == "+" || num_str == "-") return "0"; // Handle invalid intermediate cases

    std::string num = num_str; // Work on a copy

    // Handle sign separately if present (though addStrings deals with it before calling this)
    bool is_negative = false;
    if (num[0] == '-') {
        is_negative = true;
        num = num.substr(1);
    } else if (num[0] == '+') {
        num = num.substr(1);
    }
     if (num.empty()) return "0"; // Was just a sign


    // Separate integer and fractional parts
    size_t decimal_pos = num.find('.');
    std::string int_part = (decimal_pos == std::string::npos) ? num : num.substr(0, decimal_pos);
    std::string frac_part = (decimal_pos == std::string::npos) ? "" : num.substr(decimal_pos + 1);

    // 1. Remove leading zeros from integer part, unless it's just "0"
    size_t first_digit = int_part.find_first_not_of('0');
    if (std::string::npos == first_digit) { // All zeros or empty
        int_part = "0";
    } else {
        int_part = int_part.substr(first_digit);
    }
    // Handle cases like ".5" becoming "0.5" after stripping leading "0" from "0.5"
     if (int_part.empty() && decimal_pos != std::string::npos && !frac_part.empty()) {
         int_part = "0"; // Ensure "0" for ".something"
     }
     if (int_part.empty() && frac_part.empty()) { // Handle case like "0." or just "0"
         int_part = "0";
     }


    // 2. Remove trailing zeros from fractional part
    if (!frac_part.empty()) {
        size_t last_digit = frac_part.find_last_not_of('0');
        if (std::string::npos == last_digit) { // All zeros
            frac_part = ""; // Remove fractional part entirely
        } else {
            frac_part = frac_part.substr(0, last_digit + 1);
        }
    }

    // 3. Reconstruct the number
    std::string result = int_part;
    if (!frac_part.empty()) {
        result += "." + frac_part;
    }
    // Handles cases like "1." becoming "1" if frac_part is now empty

     // Final check for overall zero
    if (result == "0" || result == ".") return "0"; // Ensure result is "0", not "."

    // Re-apply sign if necessary and result is not "0"
    if (is_negative && result != "0") {
        return "-" + result;
    }

    return result;
}
