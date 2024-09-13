// grep_funcs.cpp
#include "grep_funcs.h"
#include <algorithm>
#include <stdexcept>
#include <cctype>
#include <vector>

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

bool match_alnum(const std::string& input_line) {
    return std::any_of(input_line.begin(), input_line.end(), ::isalnum);
}

bool match_pos_char_groups(const std::string& input_line, const std::string& pattern) {
    std::vector<char> groups;
    for (size_t i = 1; i < pattern.length()-1; i++){
        groups.push_back(pattern[i]);
    }

    for (char c : input_line) {
        if (std::find(groups.begin(), groups.end(), c) != groups.end()) {
            return true;
        }
    }

    return false;
}


bool match_pattern(const std::string& input_line, const std::string& pattern) {
    if (pattern.length() == 1) {
        return match_character(input_line, pattern);
    } 
    else if (pattern == "\\d") {
        return match_digit(input_line);
    } 
    else if (pattern == "\\w") {
        return match_alnum(input_line);
    }
    else if (pattern.front() == '[' && pattern.back() == ']' && pattern.length() > 2 && pattern[1] == '^') {
        return !match_pos_char_groups(input_line, '[' + pattern.substr(1, pattern.length()));
    }
    else if (pattern.front() == '[' && pattern.back() == ']') {
        return match_pos_char_groups(input_line, pattern);
    }
    else {
        throw std::runtime_error("Unhandled pattern " + pattern);
    }
}