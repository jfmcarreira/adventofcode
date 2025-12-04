#include <iostream>
#include <string>
#include <vector>

#include "../common.hpp"

enum class Direction
{
    Left,
    Right,
};

struct Action
{
    Direction dir{Direction::Left};
    std::int64_t count{0};
};

constexpr std::int64_t kMaxValue{100};

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Missing input!" << std::endl;
        return 1;
    }

    std::vector<Action> actions;
    auto parsing = [&](const std::string& line) {
        actions.push_back(
            Action{
                .dir = line[0] == 'L' ? Direction::Left : Direction::Right,
                .count = std::stoll(line.substr(1)),
            });
    };
    parse_file(argv[1], parsing);

    std::int64_t current_index{50};
    std::int64_t zero_count{0};
    for (const auto& action : actions) {
        if (action.dir == Direction::Left) {
            if (current_index == 0) {
                current_index = kMaxValue;
            }
            for (std::int64_t i = 0; i < action.count; ++i) {
                current_index -= 1;
                if (current_index == 0) {
                    ++zero_count;
                    current_index = kMaxValue;
                }
            }
        } else {
            if (current_index == kMaxValue) {
                current_index = 0;
            }
            for (std::int64_t i = 0; i < action.count; ++i) {
                current_index += 1;
                if (current_index == kMaxValue) {
                    ++zero_count;
                    current_index = 0;
                }
            }
        }
    }
    std::cout << zero_count << std::endl;
    return 0;
}

// Solution: 6254
