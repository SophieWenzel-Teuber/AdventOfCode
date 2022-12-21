#pragma once

#include <vector>
#include <string>

std::vector<std::string> split_string(std::string str_input, std::string delimiter)
{
    size_t pos = 0;
    std::string token;
    std::vector<std::string> all_parts;
    std::string input_copy = str_input;

    while ((pos = input_copy.find(delimiter)) != std::string::npos) {
        token = input_copy.substr(0, pos);
        all_parts.push_back(token);
        input_copy.erase(0, pos + delimiter.length());
    }
    all_parts.push_back(input_copy);
    return all_parts;
}
