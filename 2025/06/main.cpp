#include <algorithm>
#include <functional>
#include <ranges>
#include <string>
#include <string_view>

#include "../common.hpp"

constexpr std::string_view kExampleInput{"123 328  51 64 \n 45 64  387 23 \n  6 98  215 314\n*   +   *   +  "};

struct Operation
{
    std::vector<std::string> inputs;
    char op;
};

constexpr auto compute_operation(const Operation& operation) noexcept
{
    auto inputs = operation.inputs | std::views::transform([](const auto& text) { return from_chars(text); });
    if (operation.op == '*') {
        return std::ranges::fold_left(inputs, 1, std::multiplies<>{});
    }
    return std::ranges::fold_left(inputs, 0, std::plus<>{});
}

constexpr auto solve(std::string_view input) noexcept -> std::int64_t
{
    auto matrix = lines_to_rows(input);
    auto width = std::ranges::max_element(matrix, std::less<>{}, [](const auto& line) { return line.size(); })->size();

    for (std::int64_t i = 0; i < width; ++i) {
        auto column =
            std::views::iota(0U, matrix.size()) | std::views::transform([&](auto y) -> auto& { return matrix[y][i]; });
        if (!std::ranges::all_of(column, [](auto c) { return c == ' '; })) continue;
        std::ranges::fill(column, ',');
    }

    std::vector<Operation> operations;
    for (const auto& matrix_line : matrix) {
        std::string_view line{matrix_line};
        std::int64_t col_idx{0};
        while (!line.empty()) {
            auto line_break_pos = line.find(',');
            auto number = line.substr(0, line_break_pos);
            if (number[0] == '+' || number[0] == '*') {
                operations[col_idx].op = line[0];
            } else {
                auto input = number | std::views::reverse | std::ranges::to<std::string>();
                if (operations.size() <= col_idx) {
                    operations.push_back(Operation{.inputs = {std::move(input)}});
                } else {
                    operations[col_idx].inputs.push_back(std::move(input));
                }
            }
            if (line_break_pos == std::string_view::npos) break;
            line = line.substr(line_break_pos + 1);
            ++col_idx;
        }
    }

    std::vector<Operation> right_to_left_operations;
    right_to_left_operations.reserve(operations.size());

    for (const auto& operation : operations) {
        auto input_length = operation.inputs[0].size();
        std::vector<std::string> inputs;
        for (std::int64_t i = 0; i < input_length; ++i) {
            std::string number_chars;
            for (std::int64_t j = 0; j < operation.inputs.size(); ++j) {
                auto character = operation.inputs[j][i];
                if (character == ' ') continue;
                number_chars.push_back(character);
            }
            inputs.push_back(std::move(number_chars));
        }
        right_to_left_operations.push_back({
            .inputs = std::move(inputs),
            .op = operation.op,
        });
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
