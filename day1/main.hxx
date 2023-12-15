#include <string_view>
#include <list>
#include <iostream>
#include <algorithm>
#include <ranges>
#include <functional>

namespace aoc::day1 {
    using calibrate_t = int;

    calibrate_t trebuchet(std::string const& str);

    template<std::ranges::forward_range R>
    calibrate_t trebuchet(R &&range) {
        auto values = range | std::views::transform([](auto const& str) -> calibrate_t {
            return trebuchet(str);
        });

        return std::ranges::fold_left(values, 0, std::plus<>());
    }

    std::list<std::string> read_input(std::istream &);
}
