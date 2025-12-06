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

    std::ranges::sort(fresh_range, std::less<>{}, &Range::start);
    for (std::int64_t i = 1; i < fresh_range.size(); ++i) {
        fresh_range[i].start = std::max(fresh_range[i].start, fresh_range[i - 1].end + 1);
        if (fresh_range[i].end < fresh_range[i - 1].end) {
            fresh_range[i].end = fresh_range[i].start - 1;
        }
    }

    return std::ranges::fold_left(
        fresh_range | std::views::transform([](const auto& range) { return range.end - range.start + 1; }),
        0,
        std::plus<>{});
}

int main(int argc, char* argv[])
{
    static_assert(solve(kExampleInput) == 14);
    run_solution(argc, argv, kExampleInput, solve);
}

// 342018167474526
