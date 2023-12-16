#include <vector>
#include <iostream>
#include <ranges>

namespace aoc::day3 {
    using line_t = std::string;
    using input_t = std::vector<line_t>;
    using part_t = int;

    input_t read_input(std::istream&);

    part_t number_parts(input_t const& input);

    void add_paddings(input_t& input);

    std::vector<std::vector<bool>> has_symbol_markers(input_t const& input);
}
