#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include <fstream>
#include <iostream>
#include <string>

template<typename F>
auto parse_file(const std::filesystem::path& file_name, F&& parsing) noexcept
{
    std::ifstream input_file(file_name, std::ios_base::in);
    std::string line;
    while (std::getline(input_file, line)) {
        parsing(line);
    }
}

#endif
