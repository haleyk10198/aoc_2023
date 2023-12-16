#include <algorithm>
#include "main.hxx"

namespace aoc::day3 {
    input_t read_input(std::istream &is) {
        input_t strs;
        std::ranges::copy(std::views::istream<std::string>(is), std::back_inserter(strs));
        return strs;
    }

    part_t number_parts(input_t const &input, marker_t &&marker) {
        marked_loc_t markers = mark_symbols(input, std::forward<marker_t>(marker));
        using marked_char = std::pair<char, bool>;
        auto marked_input =
                std::views::zip(input | std::ranges::views::join, markers) | ranges::to<std::vector<marked_char>>();
        auto engine_chunks = marked_input | std::views::chunk_by([](auto &&prev, auto &&cur) {
            return isdigit(prev.first) == isdigit(cur.first);
        });
        auto engine_parts = engine_chunks | std::views::filter([](auto &&chunk) {
            return std::ranges::any_of(chunk, [](auto marked) {
                return marked.second;
            });
        }) | std::views::transform([](auto &&chunk) {
            std::string number_str = chunk | std::views::keys | ranges::to<std::string>();
            return stoi(number_str);
        });

        return std::ranges::fold_left(engine_parts, 0, std::plus());
    }

    part_t gear_parts(input_t const& input, marker_t &&marker) {
        marked_loc_t markers = mark_symbols(input, std::forward<marker_t>(marker));
        using marked_char = std::pair<char, std::optional<loc_t>>;
        auto marked_input =
                std::views::zip(input | std::ranges::views::join, markers) | ranges::to<std::vector<marked_char>>();
        auto engine_chunks = marked_input | std::views::chunk_by([](auto &&prev, auto &&cur) {
            return isdigit(prev.first) == isdigit(cur.first);
        });
        auto engine_parts = engine_chunks | std::views::filter([](auto &&chunk) {
            return std::ranges::any_of(chunk, [](marked_char const& marked) -> bool{
                return marked.second.has_value();
            });
        }) | std::views::transform([](auto&& chunk) {
            loc_t loc = (*std::ranges::find_if(chunk | std::views::values, [](marked_char::second_type const& marked) {
                return marked.has_value();
            })).value();

            std::string number_str = chunk | std::views::keys | ranges::to<std::string>();
            return std::pair{loc, stoi(number_str)};
        }) | ranges::to<std::vector>();

        std::ranges::sort(engine_parts);
        auto gears = engine_parts | std::views::chunk_by([](auto &&prev, auto &&cur) {
            return prev.first == cur.first;
        }) | std::views::transform([](auto &&chunk) {
            return std::ranges::fold_left(chunk | std::views::values, 1, std::multiplies());
        });

        return std::ranges::fold_left(gears, 0, std::plus());
    }

    void add_paddings(input_t &input) {
        input = input | std::views::transform([](auto &line) {
            return "." + line + ".";
        }) | ranges::to<input_t>();

        auto width = input.front().size();

        std::string headers(width, '.');

        input.insert(input.begin(), headers);
        input.push_back(headers);
    }

    auto neighbours(int idx, int jdx) {
        auto idxs = std::views::iota(idx - 1) | std::views::take(3);
        auto jdxs = std::views::iota(jdx - 1) | std::views::take(3);

        return std::views::cartesian_product(idxs, jdxs);
    }

    marked_loc_t mark_symbols(input_t const &input, marker_t &&pred) {
        return std::views::transform(input | std::views::enumerate, [&pred](auto &&pi) {
            auto &[idx, line] = pi;
            return line | std::views::enumerate | std::views::transform([idx, &pred](auto &&pj) -> std::optional<loc_t> {
                auto &[jdx, ch] = pj;
                if (!isdigit(ch)) {
                    return {};
                }

                auto near = neighbours(idx, jdx);
                auto iter = std::ranges::find_if(near, pred);
                return iter != near.end()? std::optional(*iter): std::optional<loc_t>{};
            }) | ranges::to<std::vector>();
        }) | std::views::join | ranges::to<std::vector>();
    }

    marked_t mark_gears(input_t const &input) {
        return input | std::views::enumerate | std::views::transform([&input](auto &&pi) -> marked_t {
            auto &[idx, line] = pi;
            return line | std::views::enumerate | std::views::transform([idx, &input](auto &&pj) {
                auto &[jdx, ch] = pj;
                if (ch != '*') {
                    return false;
                }

                auto gears = neighbours(idx, jdx) | std::views::transform([&input](auto &&loc) {
                    auto [i, j] = loc;
                    return std::pair(loc, input[i][j]);
                }) | std::views::chunk_by([](auto &&prev, auto &&cur) {
                    return (isdigit(prev.second) == isdigit(cur.second))
                           && ((prev.first).first == (cur.first).first);
                }) | std::views::filter([](auto &&chunk) {
                    return std::ranges::any_of(chunk | std::views::values, [](char near_ch) {
                        return isdigit(near_ch);
                    });
                }) | ranges::to<std::vector>();

                return gears.size() == 2;
            }) | ranges::to<marked_t>();
        }) | std::views::join | ranges::to<marked_t>();
    }
}

int main() {
    using namespace aoc::day3;

    freopen("./input.txt", "r", stdin);

    auto input = read_input(std::cin);
    add_paddings(input);

    auto mark_adj = [&input](auto &&loc) {
        auto [i, j] = loc;
        return input[i][j] != '.' && !isdigit(input[i][j]);
    };

    auto parts = number_parts(input, mark_adj);

    std::cout << parts << std::endl;

    auto gears = mark_gears(input);
    auto mark_geared = [width = input.front().size(), &gears](auto &&loc) {
        auto [i, j] = loc;
        return gears[i*width+j];
    };

    auto part2 = gear_parts(input, mark_geared);

    std::cout << part2 << std::endl;
}
