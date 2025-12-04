#include <algorithm>
#include <charconv>
#include <fstream>
#include <iostream>
#include <ranges>
#include <span>
#include <string_view>
#include <vector>

constexpr std::string_view kExampleInput{"987654321111111\n811111111111119\n234234234234278\n818181911112111"};
constexpr std::int64_t kDigitsCount{12};

constexpr auto to_number(std::span<const std::int64_t> digits, std::span<const std::int64_t> indexes) noexcept
{
    std::int64_t number{0};
    std::int64_t base{1};
    for (auto index : indexes | std::views::reverse) {
        number += digits[index] * base;
        base *= 10;
    }
    return number;
}

constexpr auto to_number(std::int64_t dozens, std::int64_t units) noexcept -> std::int64_t
{
    return dozens * 10 + units;
}

constexpr auto check_max(
    std::span<const std::int64_t> digits,
    std::span<std::int64_t> best_digits,
    std::int64_t opt_index) noexcept -> std::int64_t
{
    auto digits_count = std::ssize(digits);

    if (opt_index >= kDigitsCount) {
        return to_number(digits, best_digits);
    }

    best_digits[opt_index] = opt_index > 0 ? best_digits[opt_index - 1] + 1 : 0;
    auto end = digits_count - (kDigitsCount - opt_index) + 1;

    std::int64_t max_digit{0};
    std::int64_t max_value{0};
    while (best_digits[opt_index] < end) {
        if (digits[best_digits[opt_index]] > max_digit) {
            max_digit = digits[best_digits[opt_index]];
            auto max = check_max(digits, best_digits, opt_index + 1);
            if (max > max_value) {
                max_value = max;
            }
        }
        ++best_digits[opt_index];
    }
    return max_value;
}

constexpr auto parse_line(std::string_view input_line) noexcept -> std::int64_t
{
    auto to_numbers = std::views::transform([](const auto& c) { return static_cast<std::int64_t>(c) - 48; });
    auto digits = input_line | to_numbers | std::ranges::to<std::vector>();

    std::array<std::int64_t, kDigitsCount> best_digits;
    std::ranges::copy(std::views::iota(0) | std::views::take(kDigitsCount), best_digits.begin());

    auto max_value = check_max(digits, best_digits, 0);

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

    //  static_assert(solve(kExampleInput) == 357);

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

// 170731717900423
