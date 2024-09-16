// grep_funcs.h
#ifndef GREP_FUNCS_H
#define GREP_FUNCS_H

#include <string>

// Function declarations
bool match_character(const std::string& input_line, const std::string& pattern);
bool match_digit(const std::string& input_line);
bool match_alnum(const std::string& input_line);
bool match_pattern(const std::string& input_line, const std::string& pattern);
bool match_pos_char_groups(const std::string& input_line, const std::string& pattern);
bool match_combined_char_classes(const std::string& input_line, const std::string& pattern);

#endif // GREP_FUNCS_H