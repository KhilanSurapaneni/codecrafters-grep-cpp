#include "grep_funcs.h"
#include <algorithm>
#include <stdexcept>
#include <cctype>
#include <vector>

// Function definitions

// Checks if a given pattern character exists anywhere in the input line
bool match_character(const std::string& input_line, const std::string& pattern) {
    // Returns true if the pattern is found in the input_line
    return input_line.find(pattern) != std::string::npos;
}

// Checks if the input line contains at least one digit
bool match_digit(const std::string& input_line) {
    // Iterate over each character in the input_line
    for (char c : input_line) {
        // Use isdigit to check if the character is a digit
        if (std::isdigit(static_cast<unsigned char>(c))) {
            return true; // Return true if a digit is found
        }
    }
    return false; // Return false if no digits are found
}

// Checks if the input line contains at least one alphanumeric character
bool match_alnum(const std::string& input_line) {
    // Use any_of to check if any character in the input_line is alphanumeric
    return std::any_of(input_line.begin(), input_line.end(), ::isalnum);
}

// Matches positive character groups (e.g., [abc]) in the pattern against the input line
bool match_pos_char_groups(const std::string& input_line, const std::string& pattern) {
    // Vector to hold the characters inside the brackets
    std::vector<char> groups;
    // Extract characters from the pattern, excluding the brackets
    for (size_t i = 1; i < pattern.length() - 1; i++) {
        groups.push_back(pattern[i]);
    }

    // Check if any character in the input_line matches any character in the group
    for (char c : input_line) {
        if (std::find(groups.begin(), groups.end(), c) != groups.end()) {
            return true; // Return true if a match is found
        }
    }

    return false; // Return false if no matches are found
}

// Helper function to match the pattern against the input_line starting from the beginning
bool match_combined_char_classes_helper(const std::string& input_line, const std::string& pattern) {
    int i = 0;           // Index for the pattern
    int input_idx = 0;    // Index for the input_line

    // Loop through the pattern and input_line characters
    while (i < pattern.length() && input_idx < input_line.length()) {
        if (pattern[i] == '\\') {
            // Handle escape sequences like \d and \w
            if (i + 1 >= pattern.length()) {
                // Throw an error if the escape sequence is incomplete
                throw std::runtime_error("Incomplete escape sequence");
            }
            char esc_char = pattern[i + 1];

            if (esc_char == 'd') {
                // Expect a digit at this position in the input_line
                if (!std::isdigit(static_cast<unsigned char>(input_line[input_idx]))) {
                    return false; // Return false if the character is not a digit
                }
            }
            else if (esc_char == 'w') {
                // Expect an alphanumeric character
                if (!std::isalnum(static_cast<unsigned char>(input_line[input_idx]))) {
                    return false; // Return false if the character is not alphanumeric
                }
            }
            else {
                // Throw an error for unhandled escape sequences
                throw std::runtime_error("Unhandled escape sequence: \\" + std::string(1, esc_char));
            }

            i += 2; // Move past the escape sequence in the pattern
        } else {
            // Literal character matching
            const std::string literal_character = std::string(1, input_line[input_idx]);
            const std::string pattern_character = std::string(1, pattern[i]);

            // Use match_character to check if the characters match
            if (!match_character(literal_character, pattern_character)) {
                return false; // Return false if the characters do not match
            }

            i++; // Move to the next character in the pattern
        }

        input_idx++; // Move to the next character in the input_line
    }

    // Return true if the entire pattern has been matched
    return i == pattern.length();
}

// Attempts to match the pattern against any substring of the input_line
bool match_combined_char_classes(const std::string& input_line, const std::string& pattern) {
    size_t input_len = input_line.length();

    // Iterate over each possible starting position in the input_line
    for (size_t i = 0; i < input_len; ++i) {
        char p = pattern[0]; // Reset 'p' for each new position 'i'

        if (p == '\\') {
            // Handle escape sequences
            if (pattern.length() < 2) {
                throw std::runtime_error("Incomplete escape sequence");
            }
            char esc_char = pattern[1];

            if (esc_char == 'd') {
                // Check if the character at input_line[i] is a digit
                if (!std::isdigit(static_cast<unsigned char>(input_line[i]))) {
                    continue; // Not a digit, skip to next position
                }
            } else if (esc_char == 'w') {
                // Check if the character at input_line[i] is alphanumeric
                if (!std::isalnum(static_cast<unsigned char>(input_line[i]))) {
                    continue; // Not alphanumeric, skip to next position
                }
            } else {
                throw std::runtime_error("Unhandled escape sequence: \\" + std::string(1, esc_char));
            }
        } else {
            // Literal character matching
            if (input_line[i] != p) {
                continue; // Characters don't match, skip to next position
            }
        }

        // Call the helper function on the substring starting at position 'i'
        if (match_combined_char_classes_helper(input_line.substr(i), pattern)) {
            return true; // Match found
        }
    }
    return false; // No match found
}

bool match_start_of_string(const std::string& input_line, const std::string& pattern) {
    const std::string pattern_suffix = pattern.substr(1);
    return match_combined_char_classes_helper(input_line, pattern_suffix);
}

bool match_end_of_string(const std::string& input_line, const std::string& pattern) {
    const std::string pattern_suffix = pattern.substr(0, pattern.length() - 1);
    if (pattern_suffix.length() > input_line.length()) {
        return false; // Pattern is longer than the input line
    }
    const std::string input = input_line.substr(input_line.length() - pattern_suffix.length());
    return match_combined_char_classes_helper(input, pattern_suffix);
}


bool match_one_or_more_helper(const std::string& input_line, const std::string& prefix_pattern, char repeated_char, const std::string& suffix_pattern) {
    int prefix_idx = 0;
    int input_idx = 0;

    // Matching the Prefix Pattern Exactly
    while (prefix_idx < prefix_pattern.length() && input_idx < input_line.length()) {
        if (input_line[input_idx] == prefix_pattern[prefix_idx]) {
            prefix_idx++;
            input_idx++;
        } else {
            return false;  // Prefix does not match
        }
    }

    // Matching One or More Occurrences of the Repeated Character
    int repeat_count = 0;  // Counter to ensure at least one occurrence
    while (input_idx < input_line.length() && repeated_char == input_line[input_idx]) {
        input_idx++;
        repeat_count++;
    }
    if (repeat_count == 0) {
        return false;  // No occurrence of the character before '+'
    }

    // Matching the Suffix Pattern Exactly
    int suffix_idx = 0;
    while (suffix_idx < suffix_pattern.length() && input_idx < input_line.length()) {
        if (input_line[input_idx] == suffix_pattern[suffix_idx]) {
            suffix_idx++;
            input_idx++;
        } else {
            return false;  // Suffix does not match
        }
    }

    // Ensure Complete Match of Prefix and Suffix Patterns
    return suffix_idx == suffix_pattern.length() && prefix_idx == prefix_pattern.length();
}

bool match_one_or_more(const std::string& input_line, const std::string& pattern) {
    std::string letter = "";
    size_t prefix_idx = 0;
    size_t suffix_idx = 0;

    // Parsing the pattern to find the character before '+'
    for (size_t i = 0; i < pattern.length(); i++) {
        if (pattern[i] == '+' && i > 0) {
            letter = pattern.substr(i - 1, 1);  // Character before '+'
            prefix_idx = i - 1;                 // **Adjust index to exclude the character before '+'
            suffix_idx = i + 1;                 // Index after '+'
            break; // Break after finding the first '+'
        }
    }

    // Extracting the prefix and suffix patterns
    const std::string prefix_pattern = pattern.substr(0, prefix_idx);
    const std::string suffix_pattern = pattern.substr(suffix_idx);

    // Attempting to match the pattern within the input_line
    const char first_letter_of_prefix = prefix_pattern.empty() ? letter[0] : prefix_pattern[0];
    for (size_t i = 0; i < input_line.length(); i++) {
        char c = input_line[i];
        if (c == first_letter_of_prefix) {
            const std::string inp = input_line.substr(i);
            if (match_one_or_more_helper(inp, prefix_pattern, letter[0], suffix_pattern)) {
                return true;  // Match found
            }
        }
    }
    return false;  // No match found
}

bool contains_plus(const std::string& pattern) {
    return pattern.find('+') != std::string::npos;
}

// Main function to match the input_line against the pattern
bool match_pattern(const std::string& input_line, const std::string& pattern) {
    if (pattern.length() == 1) {
        // Single character pattern
        return match_character(input_line, pattern);
    }
    else if (pattern.length() == 2 && pattern[0] == '\\') {
        // Handle single-character escape sequences
        if (pattern == "\\d") {
            return match_digit(input_line); // Match any digit
        }
        else if (pattern == "\\w") {
            return match_alnum(input_line); // Match any alphanumeric character
        }
    }
    else if (pattern.front() == '[' && pattern.back() == ']') {
        // Handle character classes
        if (pattern.length() > 2 && pattern[1] == '^') {
            // Negative character class; invert the match result
            std::string positive_pattern = '[' + pattern.substr(2);
            return !match_pos_char_groups(input_line, positive_pattern);
        }
        else {
            // Positive character class
            return match_pos_char_groups(input_line, pattern);
        }
    }
    else if (pattern.front() == '^'){
        return match_start_of_string(input_line, pattern);
    }
    else if (pattern.back() == '$'){
        return match_end_of_string(input_line, pattern);
    }
    else if (contains_plus(pattern)) {
        return match_one_or_more(input_line, pattern);
    }
    else {
        // For other patterns, use the match function
        return match_combined_char_classes(input_line, pattern);
    }

    // Throw an error if the pattern type is unhandled
    throw std::runtime_error("Unhandled pattern " + pattern);
}