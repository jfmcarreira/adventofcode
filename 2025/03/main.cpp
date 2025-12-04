#include <algorithm>
#include <charconv>
#include <fstream>
#include <iostream>
#include <ranges>
#include <span>
#include <string_view>
#include <vector>

constexpr std::string_view kExampleInput{"987654321111111\n811111111111119\n234234234234278\n818181911112111"};

constexpr auto to_number(std::span<const std::int64_t> digits) noexcept
{
    std::int64_t number{0};
    std::int64_t base{1};
    for (auto digit : digits | std::views::reverse) {
        number += digit * base;
        base *= 10;
    }
    return number;
}

constexpr auto to_number(std::int64_t dozens, std::int64_t units) noexcept -> std::int64_t
{
    return dozens * 10 + units;
}

constexpr auto parse_line(std::string_view input_line) noexcept -> std::int64_t
{
    auto to_numbers = std::views::transform([](const auto& c) { return static_cast<std::int64_t>(c) - 48; });
    auto digits = input_line | to_numbers;

    std::int64_t max_value{0};
    auto it = digits.begin();
    while (it != digits.end()) {
        auto value = *std::ranges::max_element(
            it + 1, digits.end(), std::less<>{}, [&](auto value) { return to_number(*it, value); });
        max_value = std::max(to_number(*it, value), max_value);
        ++it;
    }

    // for (auto number : digits) {
    //     std::cout << number << " ";
    // }
    // std::cout << "- " << max_value;
    // std::cout << std::endl;

    return max_value;
}

constexpr auto solve(std::string_view input) noexcept -> std::int64_t
{
    std::int64_t result{0};
    while (!input.empty()) {
        auto line_break_pos = input.find('\n');
        result += parse_line(input.substr(0, line_break_pos));
        if (line_break_pos == std::string_view::npos) break;
        input = input.substr(line_break_pos + 1);
    }
    return result;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Missing input!" << std::endl;
        return 1;
    }
    std::string filename{argv[1]};

    static_assert(solve(kExampleInput) == 357);

    if (filename == "inline_example") {
        auto result = solve(kExampleInput);
        std::cout << result << std::endl;
        return 0;
    }

    std::ifstream file(argv[1]);
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    auto result = solve(str);
    std::cout << result << std::endl;
    return 0;
}

// 17263
