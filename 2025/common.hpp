#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include <charconv>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

constexpr auto from_chars(std::string_view input) noexcept -> std::int64_t
{
    std::int64_t value{0};
    auto [p, ec] = std::from_chars(input.begin(), input.end(), value);  // NOLINT
    return value;
}

template<typename F>
auto parse_file(const std::filesystem::path& file_name, F&& parsing) noexcept
{
    std::ifstream input_file(file_name, std::ios_base::in);
    std::string line;
    while (std::getline(input_file, line)) {
        parsing(line);
    }
}

template<typename F>
constexpr auto parse_input(std::string_view input, F&& parsing) noexcept
{
    while (!input.empty()) {
        auto line_break_pos = input.find('\n');
        parsing(input.substr(0, line_break_pos));
        if (line_break_pos == std::string_view::npos) break;
        input = input.substr(line_break_pos + 1);
    }
}

constexpr auto lines_to_rows(std::string_view input) noexcept
{
    std::vector<std::string> rows;
    while (!input.empty()) {
        auto line_break_pos = input.find('\n');
        auto line = input.substr(0, line_break_pos);
        rows.push_back(std::string{line});
        if (line_break_pos == std::string_view::npos) break;
        input = input.substr(line_break_pos + 1);
    }
    return rows;
}

template<typename F>
constexpr auto run_solution(int argc, char* argv[], std::string_view example_input, F&& solve) noexcept
{
    if (argc < 2) {
        std::cout << "Missing input!" << std::endl;
        return 1;
    }
    std::string filename{argv[1]};
    if (filename == "inline_example") {
        auto result = solve(example_input);
        std::cout << result << std::endl;
        return 0;
    }
    std::ifstream file(argv[1]);
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    auto result = solve(str);
    std::cout << result << std::endl;
    return 0;
}

#endif
