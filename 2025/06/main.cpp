#include <algorithm>
#include <functional>
#include <ranges>
#include <string>
#include <string_view>

#include "../common.hpp"

constexpr std::string_view kExampleInput{"123 328  51 64 \n 45 64  387 23 \n  6 98  215 314\n*   +   *   +  "};

template<typename T>
struct Operation
{
    std::vector<T> inputs;
    char op;
};

constexpr auto compute_operation(const Operation<std::int64_t>& operation) noexcept
{
    if (operation.op == '*') {
        return std::ranges::fold_left(operation.inputs, 1, std::multiplies<>{});
    }
    return std::ranges::fold_left(operation.inputs, 0, std::plus<>{});
}

constexpr auto solve(std::string_view input) noexcept -> std::int64_t
{
    auto matrix = lines_to_rows(input);
    auto width = std::ranges::max_element(matrix, std::less<>{}, [](const auto& line) { return line.size(); })->size();

    for (std::int64_t i = 0; i < width; ++i) {
        bool is_col{true};
        for (std::int64_t j = 0; j < matrix.size(); ++j) {
            if (matrix[j][i] != ' ') {
                is_col = false;
                break;
            }
        }
        if (is_col) {
            for (std::int64_t j = 0; j < matrix.size(); ++j) {
                matrix[j][i] = ',';
            }
        }
    }

    std::vector<Operation<std::string_view>> operations;
    for (const auto& matrix_line : matrix) {
        std::string_view line{matrix_line};
        std::int64_t col_idx{0};
        while (!line.empty()) {
            auto line_break_pos = line.find(',');
            auto number = line.substr(0, line_break_pos);
            if (number[0] == '+' || number[0] == '*') {
                operations[col_idx].op = line[0];
            } else {
                if (operations.size() <= col_idx) {
                    operations.push_back(
                        Operation<std::string_view>{
                            .inputs = {number},
                        });
                } else {
                    operations[col_idx].inputs.push_back(number);
                }
            }
            if (line_break_pos == std::string_view::npos) break;
            line = line.substr(line_break_pos + 1);
            ++col_idx;
        }
    }
    std::vector<Operation<std::int64_t>> right_to_left_operations;
    right_to_left_operations.reserve(operations.size());

    for (const auto& operation : operations) {

        Operation<std::int64_t> right_to_left_operation;
        right_to_left_operation.op = operation.op;

        auto input_length = operation.inputs[0].size();

        for (std::int64_t i = input_length - 1; i >= 0; --i) {
            std::string number_chars;
            for (std::int64_t j = 0; j < operation.inputs.size(); ++j) {
                auto character = operation.inputs[j][i];
                if (character == ' ') continue;
                number_chars.push_back(character);
            }
            right_to_left_operation.inputs.push_back(from_chars(number_chars));
        }
        right_to_left_operations.push_back(std::move(right_to_left_operation));
    }
    return std::ranges::fold_left(
        right_to_left_operations | std::views::transform(compute_operation), 0, std::plus<>{});
}

int main(int argc, char* argv[])
{
    static_assert(solve(kExampleInput) == 3263827);
    run_solution(argc, argv, kExampleInput, solve);
}

// 7450962489289
