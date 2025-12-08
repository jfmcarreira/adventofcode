#include <algorithm>
#include <functional>
#include <ranges>
#include <string_view>
#include <vector>

#include "../common.hpp"

constexpr std::string_view kExampleInput{
    ".......S.......\n"
    "...............\n"
    ".......^.......\n"
    "...............\n"
    "......^.^......\n"
    "...............\n"
    ".....^.^.^.....\n"
    "...............\n"
    "....^.^...^....\n"
    "...............\n"
    "...^.^...^.^...\n"
    "...............\n"
    "..^...^.....^..\n"
    "...............\n"
    ".^.^.^.^.^...^.\n"
    "..............."};

constexpr char kStart{'S'};
constexpr char kEmpty{'.'};
constexpr char kSplitter{'^'};

constexpr std::int64_t kSplitterIndex{-1};

constexpr auto solve(std::string_view input) noexcept -> std::int64_t
{
    auto map = [&]() {
        auto map = lines_to_rows(input);
        auto line_width = map[0].size();

        std::vector<std::vector<std::int64_t>> index_map;
        index_map.resize(map.size());
        for (auto& line : index_map) {
            line.resize(line_width);
        }

        std::ranges::transform(
            map | std::views::join, (index_map | std::views::join).begin(), [](auto character) -> std::int64_t {
                switch (character) {
                    case kEmpty:
                        return 0;
                    case kStart:
                        return 1;
                    case kSplitter:
                        return kSplitterIndex;
                }
                return 0;
            });
        return index_map;
    }();
    for (std::int64_t y = 1; y < map.size(); ++y) {
        auto line_width = map[y].size();
        for (std::int64_t x = 0; x < line_width; ++x) {
            if (map[y - 1][x] == 0) continue;
            if (map[y - 1][x] == kSplitterIndex) continue;
            if (map[y][x] != kSplitterIndex) {
                map[y][x] += map[y - 1][x];
                continue;
            }

            auto append = [&](auto delta) {
                if (x + delta < 0 || x + delta >= line_width) return;
                auto& pos = map[y][x + delta];
                pos += map[y - 1][x];
            };
            append(+1);
            append(-1);
        }
    }
    return std::ranges::fold_left(map.back() | std::views::transform([](auto v) { return v; }), 0, std::plus<>{});
}

int main(int argc, char* argv[])
{
    static_assert(solve(kExampleInput) == 40);
    run_solution(argc, argv, kExampleInput, solve);
}

// 40999072541589
