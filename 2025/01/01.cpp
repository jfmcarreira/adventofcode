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

auto action_to_offset(const Action& action) noexcept
{
    return action.dir == Direction::Left ? -action.count : action.count;
}

auto wrap_index(std::int64_t index) noexcept
{
    constexpr std::int64_t kMaxValue{100};
    if (index >= 0) return index % kMaxValue;
    auto wrapped = index % kMaxValue;
    if (wrapped < 0) return wrapped + kMaxValue;
    return wrapped;
}

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
        current_index = wrap_index(current_index + action_to_offset(action));
        if (current_index == 0) {
            ++zero_count;
        }
    }
    std::cout << zero_count << std::endl;
    return 0;
}
