#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>
#include <ranges>
#include <string_view>
#include <utility>
#include <vector>

#include "../common.hpp"

constexpr std::string_view kExampleInput{
    "162,817,812\n"
    "57,618,57\n"
    "906,360,560\n"
    "592,479,940\n"
    "352,342,300\n"
    "466,668,158\n"
    "542,29,236\n"
    "431,825,988\n"
    "739,650,466\n"
    "52,470,668\n"
    "216,146,977\n"
    "819,987,18\n"
    "117,168,530\n"
    "805,96,715\n"
    "346,949,466\n"
    "970,615,88\n"
    "941,993,340\n"
    "862,61,35\n"
    "984,92,344\n"
    "425,690,689"};

struct Coord
{
    std::int64_t x;
    std::int64_t y;
    std::int64_t z;
    std::int64_t group;
};

constexpr auto operator-(const Coord& lhs, const Coord& rhs) noexcept
{
    return Coord{rhs.x - lhs.x, rhs.y - lhs.y, lhs.z - rhs.z};
}

std::ostream& operator<<(std::ostream& os, const Coord& point)
{
    os << "(" << point.x << ", " << point.y << ", " << point.x << ")";
    os << " [" << point.group << "]";
    return os;
}

struct Distance
{
    std::int64_t lhs;
    std::int64_t rhs;
    double distance;
};

std::ostream& operator<<(std::ostream& os, const Distance& point)
{
    os << "(" << point.lhs << ", " << point.rhs << ", " << point.distance << ")";
    return os;
}

constexpr auto measure_distance(const Coord& lhs, const Coord& rhs) noexcept
{
    auto diff = rhs - lhs;
    return (diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
}

constexpr auto compute_distances(const std::vector<Coord>& junctions) noexcept
{
    auto count = junctions.size();
    std::vector<Distance> distances;
    distances.reserve(count * count);
    for (std::int64_t i = 0; i < count; ++i) {
        for (std::int64_t j = 0; j < count; ++j) {
            Distance distance;
            distance.lhs = i;
            distance.rhs = j;
            if (i >= j) {
                continue;
            }
            distance.distance = measure_distance(junctions[i], junctions[j]);
            distances.push_back(std::move(distance));
        }
    }
    std::ranges::sort(distances, std::less<>{}, &Distance::distance);
    return distances;
}

constexpr auto match_circuits(const std::vector<Distance>& distances, std::vector<Coord>& junctions) noexcept
{
    std::int64_t match_count{1};
    while (true) {
        std::optional<std::pair<std::int64_t, std::int64_t>> mapping;
        for (std::int64_t i = 0; i <= match_count; ++i) {
            const auto& distance = distances[i];
            if (junctions[distance.lhs].group == junctions[distance.rhs].group) continue;
            mapping = std::make_pair(
                std::max(junctions[distance.lhs].group, junctions[distance.rhs].group),
                std::min(junctions[distance.lhs].group, junctions[distance.rhs].group));
            break;
        }
        if (mapping.has_value()) {
            for (auto& junction : junctions) {
                if (junction.group == mapping->first) {
                    junction.group = mapping->second;
                }
            }
            continue;
        }
        auto same_group = std::all_of(
            junctions.begin() + 1, junctions.end(), [&](const auto& j) { return j.group == junctions.front().group; });
        if (same_group) break;
        ++match_count;
    }
    return match_count;
}

constexpr auto solve(std::string_view input) noexcept -> std::int64_t
{
    std::int64_t group_idx{0};
    std::vector<Coord> junctions;
    parse_input(input, [&](auto line) {
        auto comma_index = line.find(',');
        auto x = from_chars(line.substr(0, comma_index));

        line = line.substr(comma_index + 1);
        comma_index = line.find(',');

        junctions.push_back(
            Coord{
                .x = x,
                .y = from_chars(line.substr(0, comma_index)),
                .z = from_chars(line.substr(comma_index + 1)),
                .group = group_idx++,
            });
    });
    auto distances = compute_distances(junctions);
    auto match_count = match_circuits(distances, junctions);
    const auto& distance = distances[match_count];
    return junctions[distance.lhs].x * junctions[distance.rhs].x;
}

int main(int argc, char* argv[])
{
    static_assert(solve(kExampleInput) == 25272);
    run_solution(argc, argv, kExampleInput, solve);
}
