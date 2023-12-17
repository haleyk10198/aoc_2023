#include <algorithm>
#include <iostream>
#include "main.hxx"
#include <range/v3/to_container.hpp>
#include "commons.hxx"

using std::operator ""sv;

namespace {
    template<std::ranges::forward_range R>
    std::vector<aoc::day5::mapping::category_t> collect_ints(R &&range) {
        return std::forward<R>(range) | std::views::split(' ')
               | std::views::transform([](auto &&num_str) {
            return (std::string) ((std::string_view) num_str);
        })
               | std::views::filter([](auto &&str) { return !str.empty(); })
               | std::views::transform([](std::string const &str) {
            return std::stoll(str);
        })
               | ranges::to<std::vector>();
    }
}

aoc::day5::part1_input aoc::day5::part1_input::read_input(std::istream &is) {
    std::vector<std::string> lines;
    std::ranges::copy(std::views::istream<CompleteLine>(is), std::back_inserter(lines));
    auto chunks = lines | std::views::chunk_by([](auto &&, std::string const &cur) {
        return !cur.empty();
    });

    auto seeds = collect_ints(chunks.front().front()
                              | std::views::drop_while([](auto &&ch) { return ch != ':'; })
                              | std::views::drop(1));

    std::list<type_mapping> all_mappings = chunks | std::views::drop(1)
                                           | std::views::transform([]<typename Ls>(Ls &&lines) {
        auto mappings = std::forward<Ls>(lines) | std::views::drop(2) |
                        std::views::transform([]<typename T>(T &&line) {
                            return collect_ints(std::forward<T>(line));
                        }) |
                        std::views::transform([]<typename T>(T &&line) {
                            mapping::category_t dst = line[0];
                            mapping::category_t src = line[1];
                            mapping::span_t span = line[2];

                            return mapping{
                                    .src_ = src,
                                    .span_ = span,
                                    .dst_ = dst
                            };
                        }) | ranges::to<std::vector>();

        mappings.emplace_back(0, 0, 0);
        std::ranges::sort(mappings);
        return type_mapping{.mappings_ = mappings};
    }) | ranges::to<std::list>();

    return part1_input{
            .seeds_ = seeds,
            .mappings_ = all_mappings
    };
}

aoc::day5::mapping::category_t aoc::day5::part1_input::solve() const {
    return std::ranges::min(seeds_ | std::views::transform([this]<typename T>(T &&seed) {
        return std::ranges::fold_left(mappings_, std::forward<T>(seed), [](auto &&seed, auto &&mapping) {
            return mapping.map(seed);
        });
    }));
}

bool aoc::day5::mapping::contains(aoc::day5::mapping::category_t category) const {
    return src_ <= category && src_ + span_ > category;
}

aoc::day5::mapping::category_t aoc::day5::mapping::map(aoc::day5::mapping::category_t src) const {
    return contains(src) ? (src - src_ + dst_) : src;
}

aoc::day5::mapping::category_t aoc::day5::type_mapping::map(aoc::day5::mapping::category_t src) const {
    auto mapping = std::ranges::upper_bound(mappings_, src, std::less(), [](auto &&mapping) { return mapping.src_; });
    return mapping == mappings_.begin() ? src : (--mapping)->map(src);
}

std::vector<aoc::day5::type_mapping::range_t>
aoc::day5::type_mapping::map_range(const aoc::day5::type_mapping::range_t &range) const {
    auto exclusive_end = range.first + range.second;
    auto mapping = std::ranges::lower_bound(mappings_, exclusive_end, std::less(),
                                            [](auto &&mapping) { return mapping.src_; });
    std::vector<aoc::day5::type_mapping::range_t> ret;

    while (exclusive_end != range.first) {
        --mapping;
        auto continuous_range_start = std::max(mapping->src_ + mapping->span_, range.first);
        if (continuous_range_start < exclusive_end) {
            auto span = exclusive_end - continuous_range_start;
            ret.emplace_back(continuous_range_start, span);
            exclusive_end = continuous_range_start;
        }

        if (mapping->contains(exclusive_end - 1)) {
            auto subrange_start = std::max(mapping->src_, range.first);
            auto span = exclusive_end - subrange_start;

            ret.emplace_back(mapping->map(subrange_start), span);
            exclusive_end = subrange_start;
        }  else break;
    }

    return ret;
}

int main() {
    using namespace aoc::day5;

    freopen("./input.txt", "r", stdin);

    auto input = part1_input::read_input(std::cin);

    std::cout << input.solve() << std::endl;

    auto part2 = part2_input::transform(input);

    std::cout << part2.solve() << std::endl;
}

aoc::day5::part2_input aoc::day5::part2_input::transform(const aoc::day5::part1_input &part1) {
    return {
            .seeds_ = part1.seeds_ | std::views::chunk(2) |
                     std::views::transform([](auto &&chunk) { return type_mapping::range_t(chunk[0], chunk[1]); })
                     | ranges::to<std::vector>(),
            .mappings_ = part1.mappings_
    };
}

aoc::day5::mapping::category_t aoc::day5::part2_input::solve() const {
    return std::ranges::min(std::ranges::fold_left(mappings_, seeds_, [](auto&& ranges, auto&& mapping) {
        auto mapped = ranges | std::views::transform([&mapping](auto&& range) {
            return mapping.map_range(range);
        }) | std::views::join | ranges::to<std::vector>();

        return mapped;
    })).first;
}
