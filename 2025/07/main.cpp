#include <algorithm>
#include <functional>
#include <ranges>
#include <string>
#include <string_view>

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
constexpr char kSplitter{'^'};
constexpr char kBeam{'|'};

constexpr auto solve(std::string_view input) noexcept -> std::int64_t
{
    auto map = lines_to_rows(input);
    auto start_index = map[0].find(kStart);
    if (start_index == std::string::npos) return 0;

    map[1][start_index] = kBeam;

    std::int64_t result{0};
    for (std::int64_t y = 2; y < map.size(); ++y) {
        auto line_width = map[y].size();
        for (std::int64_t x = 0; x < line_width; ++x) {
            if (map[y - 1][x] != kBeam) continue;
            if (map[y][x] != kSplitter) {
                map[y][x] = kBeam;
                continue;
            }
            ++result;
            if (x > 0) {
                map[y][x - 1] = kBeam;
            }
            if (x < line_width - 1) {
                map[y][x + 1] = kBeam;
            }
        }
    }
    // for (const auto& line : map) {
    //     std::cout << line << std::endl;
    // }
    return result;
}

int main(int argc, char* argv[])
{
    static_assert(solve(kExampleInput) == 21);
    run_solution(argc, argv, kExampleInput, solve);
}

// 1640
