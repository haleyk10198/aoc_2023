#include <algorithm>
#include "main.hxx"
#include <range/v3/to_container.hpp>
#include <charconv>

namespace aoc::day3 {
    input_t read_input(std::istream &is) {
        input_t strs;
        std::ranges::copy(std::views::istream<std::string>(is), std::back_inserter(strs));
        return strs;
    }

    part_t number_parts(input_t const &input) {
        std::vector<bool> markers = has_symbol_markers(input) | std::ranges::views::join | ranges::to<std::vector>();
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

    void add_paddings(input_t &input) {
        input = input | std::views::transform([](auto &line) {
            return "." + line + ".";
        }) | ranges::to<input_t>();

        auto width = input.front().size();

        std::string headers(width, '.');

        input.insert(input.begin(), headers);
        input.push_back(headers);
    }

    std::vector<std::vector<bool>> has_symbol_markers(input_t const &input) {
        return std::views::transform(input | std::views::enumerate, [&input](auto &&pi) {
            auto &[idx, line] = pi;
            return line | std::views::enumerate | std::views::transform([idx, &input](auto &&pj) -> bool {
                auto &[jdx, ch] = pj;
                if (!isdigit(ch)) {
                    return false;
                }

                auto idxs = std::views::iota(idx - 1) | std::views::take(3);
                auto jdxs = std::views::iota(jdx - 1) | std::views::take(3);

                return std::ranges::any_of(
                        std::views::cartesian_product(idxs, jdxs), [&input](auto const &loc) -> bool {
                            auto [i, j] = loc;
                            return input[i][j] != '.' && !isdigit(input[i][j]);
                        });
            }) | ranges::to<std::vector<bool>>();
        }) | ranges::to<std::vector>();
    }
}

int main() {
    using namespace aoc::day3;

    freopen("./input.txt", "r", stdin);

    auto lines = read_input(std::cin);
    add_paddings(lines);
    auto parts = number_parts(lines);

    std::cout << parts << std::endl;
}
