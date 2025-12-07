#include <algorithm>
#include <functional>
#include <ranges>
#include <string_view>

#include "../common.hpp"

constexpr std::string_view kExampleInput{"123 328  51 64 \n 45 64  387 23 \n  6 98  215 314\n*   +   *   +  "};

struct Operation
{
    std::vector<std::int64_t> inputs;
    char op;
};

constexpr auto compute_operation(const Operation& operation) noexcept
{
    if (operation.op == '*') {
        return std::ranges::fold_left(operation.inputs, 1, std::multiplies<>{});
    }
    return std::ranges::fold_left(operation.inputs, 0, std::plus<>{});
}

constexpr auto solve(std::string_view input) noexcept -> std::int64_t
{
    std::vector<Operation> operations;

    parse_input(input, [&operations](std::string_view line) {
        std::int64_t col_idx{0};
        while (!line.empty()) {
            if (line[0] == ' ') {
                line = line.substr(1);
                continue;
            }
            if (line[0] == '+' || line[0] == '*') {
                operations[col_idx].op = line[0];
                line = line.substr(1);
            } else {
                auto line_break_pos = line.find(' ');
                auto number = from_chars(line.substr(0, line_break_pos));
                if (operations.size() <= col_idx) {
                    operations.push_back(
                        Operation{
                            .inputs = {number},
                        });
                } else {
                    operations[col_idx].inputs.push_back(number);
                }
                if (line_break_pos == std::string_view::npos) return;
                line = line.substr(line_break_pos + 1);
            }
            ++col_idx;
        }
    });

    return std::ranges::fold_left(operations | std::views::transform(compute_operation), 0, std::plus<>{});
}

int main(int argc, char* argv[])
{
    static_assert(solve(kExampleInput) == 4277556);
    run_solution(argc, argv, kExampleInput, solve);
}

// 4405895212738
