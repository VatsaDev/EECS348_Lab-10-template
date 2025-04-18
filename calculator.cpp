// calculator.cpp
#include "calculator.hpp"
#include <string>
#include <vector>
#include <iostream> // Include for debugging if needed, remove later
#include <algorithm> // For std::reverse, std::max

// --- Validation Logic ---

bool isValidDouble(const std::string &expression) {
    if (expression.empty()) {
        return false;
    }

    int i = 0;
    int n = expression.length();
    bool hasSign = false;
    bool hasDecimal = false;
    bool hasDigit = false;

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
            expectingDigitAfterDecimal = false; // Reset expectation if we see a digit
        } else if (expression[i] == '.') {
            if (hasDecimal) return false; // Multiple decimal points
            hasDecimal = true;
            expectingDigitAfterDecimal = true; // Must see a digit next
        } else {
            return false; // Invalid character
        }
    }

    // Final checks
    if (!hasDigit) return false; // Must have at least one digit somewhere
    if (expectingDigitAfterDecimal) return false; // Ended with a decimal point without trailing digit

    return true;
}


// --- String Arithmetic Logic ---

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

    // Compare integer parts
    if (int1.length() > int2.length()) return 1;
    if (int1.length() < int2.length()) return -1;
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
    int j = num2.length() - 1;
    int carry = 0;

    while (i >= 0 || j >= 0 || carry) {
        int digit1 = 0;
        int digit2 = 0;

        if (i >= 0) {
            if (num1[i] == '.') {
                result += '.';
                i--;
                j--; // Align decimal points
                continue; // Skip calculation for decimal point
            }
            digit1 = num1[i] - '0';
            i--;
        }

        if (j >= 0) {
             if (num2[j] == '.') {
                // This condition should ideally not be hit if alignment is correct
                // but added for robustness. If num1 didn't have '.', add it now.
                if (result.find('.') == std::string::npos) {
                   result += '.';
                }
                j--;
                continue;
            }
            digit2 = num2[j] - '0';
            j--;
        }


        int sum = digit1 + digit2 + carry;
        result += std::to_string(sum % 10);
        carry = sum / 10;
    }
    std::reverse(result.begin(), result.end());
    return result.empty() ? "0" : result;
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

        int digit1 = num1[i] - '0';
        int digit2 = (i < n2 && num2[i] != '.') ? (num2[i] - '0') : 0; // Handle potential length diff if alignment fails (shouldn't happen)

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
    // Remove leading zeros potentially created by subtraction
    // size_t first_digit = result.find_first_not_of('0');
    // if (std::string::npos == first_digit) return "0"; // Result is zero
    // if (result[first_digit] == '.') first_digit--; // Keep the zero before decimal if like "0.5"
    // result = result.substr(first_digit);

    return result; // Normalization will happen later
}

// Main addition function
std::string addStrings(const std::string &n1, const std::string &n2) {
    // Handle signs
    bool neg1 = n1[0] == '-';
    bool neg2 = n2[0] == '-';
    std::string absNum1 = neg1 ? n1.substr(1) : (n1[0] == '+' ? n1.substr(1) : n1);
    std::string absNum2 = neg2 ? n2.substr(1) : (n2[0] == '+' ? n2.substr(1) : n2);

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
    else if (dec1Pos != std::string::npos || dec2Pos != std::string::npos) {
         // Handle case where one had decimal but result has none, e.g., 1.5 + 0.5
         // Need to ensure add/sub handles this structure if decimal exists.
         // Simpler: only add "." if frac parts were non-empty initially OR maxFracLen > 0
         if(!fracPart1.empty() || !fracPart2.empty()) {
              alignedNum1 += "." + fracPart1; // Add empty frac part if needed
         }
    }


    std::string alignedNum2 = intPart2;
     if (maxFracLen > 0) alignedNum2 += "." + fracPart2;
     else if (dec1Pos != std::string::npos || dec2Pos != std::string::npos) {
        if(!fracPart1.empty() || !fracPart2.empty()) {
             alignedNum2 += "." + fracPart2;
        }
     }
    // If neither had decimal initially and maxFracLen is 0, no decimal is added.


    std::string resultAbs = "";
    bool resultNeg = false;

    if (neg1 == neg2) { // Both positive or both negative
        resultAbs = addAbsoluteStrings(alignedNum1, alignedNum2);
        resultNeg = neg1; // Result has the same sign
    } else { // Different signs - perform subtraction
        int cmp = compareAbsoluteStrings(alignedNum1, alignedNum2);
        if (cmp == 0) {
            resultAbs = "0"; // Numbers are equal magnitude, opposite sign
            resultNeg = false;
        } else if (cmp > 0) { // abs(num1) > abs(num2)
            resultAbs = subtractAbsoluteStrings(alignedNum1, alignedNum2);
            resultNeg = neg1; // Result has the sign of the larger magnitude number (num1)
        } else { // abs(num1) < abs(num2)
            resultAbs = subtractAbsoluteStrings(alignedNum2, alignedNum1);
            resultNeg = neg2; // Result has the sign of the larger magnitude number (num2)
        }
    }

    // Normalize and prepend sign if needed
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
    std::string num = num_str; // Work on a copy

    // Separate integer and fractional parts
    size_t decimal_pos = num.find('.');
    std::string int_part = (decimal_pos == std::string::npos) ? num : num.substr(0, decimal_pos);
    std::string frac_part = (decimal_pos == std::string::npos) ? "" : num.substr(decimal_pos + 1);

    // 1. Remove leading zeros from integer part, unless it's just "0"
    size_t first_digit = int_part.find_first_not_of('0');
    if (std::string::npos == first_digit) { // All zeros
        int_part = "0";
    } else {
        int_part = int_part.substr(first_digit);
    }
    // Handle cases like ".5" becoming "0.5" after stripping leading "0" from "0.5"
     if (int_part.empty() && decimal_pos != std::string::npos) {
         int_part = "0";
     }


    // 2. Remove trailing zeros from fractional part
    if (!frac_part.empty()) {
        size_t last_digit = frac_part.find_last_not_of('0');
        if (std::string::npos == last_digit) { // All zeros
            frac_part = "";
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

     // Final check for just "0" or "-0" etc.
    if (result == "0" || result == ".0" || result == "0." || result == ".") return "0";


    return result;
}
