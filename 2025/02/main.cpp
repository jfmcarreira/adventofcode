#include <algorithm>
#include <charconv>
#include <fstream>
#include <iostream>
#include <ranges>
#include <span>
#include <string_view>
#include <vector>

constexpr std::string_view kExampleInput{
    "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-"
    "565659,824824821-824824827,2121212118-2121212124"};

struct Range
{
    std::int64_t start{0};
    std::int64_t end{0};
};

std::ostream& operator<<(std::ostream& os, const Range& range)
{
    os << "(" << range.start << ", " << range.end << ")";
    return os;
}

constexpr auto from_chars(std::string_view input) noexcept -> std::int64_t
{
    std::int64_t value{0};
    auto [p, ec] = std::from_chars(input.begin(), input.end(), value);  // NOLINT
    return value;
}

constexpr auto to_digits(std::int64_t number) noexcept
{
    std::vector<std::int64_t> digits;
    while (number > 0) {
        digits.push_back(number % 10);
        number /= 10;
    }
    std::ranges::reverse(digits);
    return digits;
}

constexpr auto split_by_comma(std::string_view input) noexcept -> std::pair<std::int64_t, Range>
{
    auto comma_index = input.find(',');
    auto range_str = input.substr(0, comma_index);
    auto dash_index = range_str.find('-');
    Range id_range{
        .start = from_chars(range_str.substr(0, dash_index)),
        .end = from_chars(range_str.substr(dash_index + 1)),
    };
    return std::make_pair(comma_index == std::string_view::npos ? input.size() : comma_index + 1, id_range);
}

constexpr auto is_invalid_id_digits(std::span<const std::int64_t> digits, std::int64_t repeat_count) noexcept
{
    auto count = digits.size();
    if (count % repeat_count != 0) return false;
    auto reference = std::span{digits}.subspan(0, count / repeat_count);
    auto checks = std::views::iota(1) | std::views::take(repeat_count - 1);
    return std::ranges::all_of(checks, [&](auto index) {
        return std::ranges::equal(
            reference, std::span{digits}.subspan(index * count / repeat_count, count / repeat_count));
    });
}

constexpr auto is_invalid_id(std::int64_t number) noexcept
{
    auto digits = to_digits(number);
    auto repeats = std::views::iota(2ULL, digits.size() + 1);
    return std::ranges::any_of(repeats, [&](auto repeat_count) { return is_invalid_id_digits(digits, repeat_count); });
}

constexpr auto day_two_puzzle(std::string_view input) noexcept -> std::int64_t
{
    std::vector<Range> id_ranges;
    std::int64_t index{0};
    while (index < input.size()) {
        auto [parsed, range] = split_by_comma(input.substr(index));
        index += parsed;
        id_ranges.push_back(range);
    }

    auto compute_invalid_ids = [](const auto& range) {
        return std::views::iota(range.start, range.end + 1) | std::views::filter(is_invalid_id);
    };
    auto invalid_ids = id_ranges | std::views::transform(compute_invalid_ids) | std::views::join;
    // for (auto number : invalid_ids) {
    //     std::cout << number << std::endl;
    // }
    return std::ranges::fold_left(invalid_ids, 0, std::plus<>{});
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Missing input!" << std::endl;
        return 1;
    }

    static_assert(day_two_puzzle(kExampleInput) == 4174379265);

    std::ifstream file(argv[1]);
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    auto result = day_two_puzzle(str);
    std::cout << result << std::endl;

    return 0;
}

// 27469417404
