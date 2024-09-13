// grep_funcs.cpp
#include "grep_funcs.h"
#include <algorithm>
#include <stdexcept>
#include <cctype>

// Function definitions
bool match_character(const std::string& input_line, const std::string& pattern) {
    return input_line.find(pattern) != std::string::npos;
}

bool match_digit(const std::string& input_line) {
    for (char c : input_line) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            return true;
        }
    }
    return false;
}

bool match_pattern(const std::string& input_line, const std::string& pattern) {
    if (pattern.length() == 1) {
        return match_character(input_line, pattern);
    } else if (pattern == "\\d") {
        return match_digit(input_line);
    } else {
        throw std::runtime_error("Unhandled pattern " + pattern);
    }
}