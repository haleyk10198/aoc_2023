#include "main.hxx"
#include <sstream>
#include <numeric>
#include <range/v3/view/concat.hpp>
#include <algorithm>

namespace aoc::day1 {

    std::list<std::string> read_input(std::istream &in) {
        std::list<std::string> inputs;
        std::ranges::copy(std::ranges::istream_view<std::string>(in), std::back_inserter(inputs));
        return inputs;
    }

    calibrate_t trebuchet(std::string const &str) {
        using digit_md_t = std::pair<std::string, int>;
        static const auto english_digits = {
                digit_md_t{"one", 1},
                {"two", 2},
                {"three", 3},
                {"four", 4},
                {"five", 5},
                {"six", 6},
                {"seven", 7},
                {"eight", 8},
                {"nine", 9}
        };

        static const auto western_digits = std::ranges::iota_view(0) | std::ranges::views::take(10)
                                           | std::views::transform([](auto const &x) -> digit_md_t {
            return {std::to_string(x), x};
        });

        static const auto all_digit_mds = ranges::views::concat(english_digits, western_digits);

        static const auto md_value = [](const digit_md_t &md) {
            return md.second;
        };

        struct Match {
            calibrate_t value;
            std::string::size_type pos;

            [[nodiscard]] auto get_pos() const {
                return pos;
            }
        };

        static const auto find_substr_by = [](auto pred, auto pos_cmp) -> calibrate_t {
            auto all_matches = std::ranges::views::transform(all_digit_mds, [&pred](const auto &md) {
                return Match{.value = md_value(md), .pos = std::invoke(pred, md.first)};
            }) | std::ranges::views::filter([](const Match &match) {
                return match.pos != std::string::npos;
            });
            Match best_match = std::ranges::min(all_matches, pos_cmp, &Match::get_pos);

            return best_match.value;
        };

        auto fst = find_substr_by([&str](std::string const &pattern) {
            return str.find(pattern);
        }, std::less());

        auto snd = find_substr_by([&str](std::string const &pattern) {
            return str.rfind(pattern);
        }, std::greater());

        std::stringstream ss;
        ss << fst << snd;
        calibrate_t value;
        ss >> value;

        return value;
    }

}

using namespace aoc::day1;

int main() {
    freopen("./input.txt", "r", stdin);

    auto strs = read_input(std::cin);

    std::cout << trebuchet(strs) << std::endl;

    return 0;
}