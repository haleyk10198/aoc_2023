#include <vector>
#include <iostream>
#include <ranges>
#include <range/v3/to_container.hpp>

namespace aoc::day3 {
    using line_t = std::string;
    using input_t = std::vector<line_t>;
    using part_t = int;

    input_t read_input(std::istream&);

    using loc_t = std::pair<int, int>;
    using marker_t = std::function<bool(loc_t)>;
    part_t number_parts(input_t const& input, marker_t&& marker);

    void add_paddings(input_t& input);

    using marked_t = std::vector<bool>;
    marked_t mark_gears(input_t const& input);

    using marked_loc_t = std::vector<std::optional<loc_t>>;
    marked_loc_t mark_symbols(input_t const& input, marker_t&& marker);
}
