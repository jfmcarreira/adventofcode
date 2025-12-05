#include <string_view>

#include "../common.hpp"

constexpr std::string_view kExampleInput{
    "..@@.@@@@.\n@@@.@.@.@@\n@@@@@.@.@@\n@.@@@@..@.\n@@.@@@@.@@\n.@@@@@@@.@\n.@.@.@.@@@\n@.@@@.@@@@\n.@@@@@@@@.\n@.@.@@"
    "@.@."};

constexpr auto kRolls{'@'};

constexpr auto solve(std::string_view input) noexcept -> std::int64_t
{
    auto map = lines_to_rows(input);
    if (map.empty()) return 0;
    auto rows = std::ssize(map);
    auto cols = std::ssize(map.front());

    auto count_rolls = [](const auto& map, auto y_pos, auto x_pos) {
        auto rows = std::ssize(map);
        auto cols = std::ssize(map.front());
        std::int64_t count{0};
        for (std::int64_t y = y_pos - 1; y <= y_pos + 1; ++y) {
            for (std::int64_t x = x_pos + -1; x <= x_pos + 1; ++x) {
                if (!(y >= 0 && y < rows && x >= 0 && x < cols)) continue;
                if (map[y][x] != kRolls) continue;
                ++count;
            }
        }
        return count - 1;
    };

    std::int64_t result{0};
    for (std::int64_t y = 0; y < rows; ++y) {
        for (std::int64_t x = 0; x < cols; ++x) {
            if (map[y][x] != kRolls) continue;
            auto count = count_rolls(map, y, x);
            if (count < 4) {
                ++result;
            }
        }
    }
    return result;
}

int main(int argc, char* argv[])
{
    static_assert(solve(kExampleInput) == 13);
    run_solution(argc, argv, kExampleInput, solve);
}
