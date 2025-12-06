#include <algorithm>
#include <ranges>
#include <string_view>

#include "../common.hpp"

constexpr std::string_view kExampleInput{"3-5\n10-14\n16-20\n12-18\n\n1\n5\n8\n11\n17\n32"};

struct Range
{
    std::int64_t start;
    std::int64_t end;
};

constexpr auto solve(std::string_view input) noexcept -> std::int64_t
{
    std::vector<Range> fresh_range;
    while (!input.empty()) {
        auto line_break_pos = input.find('\n');
        auto line = input.substr(0, line_break_pos);
        if (line.empty()) break;

        auto dash_index = line.find('-');
        fresh_range.push_back({
            .start = from_chars(line.substr(0, dash_index)),
            .end = from_chars(line.substr(dash_index + 1)),
        });
        if (line_break_pos == std::string_view::npos) break;
        input = input.substr(line_break_pos + 1);
    }

    std::vector<std::int64_t> available;
    while (!input.empty()) {
        auto line_break_pos = input.find('\n');
        auto line = input.substr(0, line_break_pos);
        available.push_back(from_chars(line));
        if (line_break_pos == std::string_view::npos) break;
        input = input.substr(line_break_pos + 1);
    }

    auto is_fresh = [&fresh_range](const auto& ingredient) {
        auto found = std::ranges::any_of(
            fresh_range, [&](const auto& range) { return ingredient >= range.start && ingredient <= range.end; });
        return found ? 1 : 0;
    };
    return std::ranges::fold_left(available | std::views::transform(is_fresh), 0, std::plus<>{});
}

int main(int argc, char* argv[])
{
    static_assert(solve(kExampleInput) == 3);
    run_solution(argc, argv, kExampleInput, solve);
}
