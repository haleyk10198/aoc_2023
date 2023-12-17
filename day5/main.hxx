#include <ranges>
#include <list>
#include <vector>

namespace aoc::day5 {
    struct mapping {
        using category_t = long long int;
        using span_t = long long int;

        category_t src_;
        span_t span_;
        category_t dst_;

        auto operator<=>(const mapping &) const = default;

        [[nodiscard]] bool contains(category_t) const;

        [[nodiscard]] category_t map(category_t) const;
    };

    struct type_mapping {
        std::vector<mapping> mappings_;

        [[nodiscard]] mapping::category_t map(mapping::category_t src) const;

        using range_t = std::pair<mapping::category_t, mapping::span_t>;
        [[nodiscard]] std::vector<range_t> map_range(range_t const& range_) const;
    };

    struct part1_input {
        std::vector<mapping::category_t> seeds_;
        std::list<type_mapping> mappings_;

        static part1_input read_input(std::istream &is);

        [[nodiscard]] mapping::category_t solve() const;
    };

    struct part2_input {
        std::vector<type_mapping::range_t> seeds_;
        std::list<type_mapping> mappings_;

        static part2_input transform(part1_input const&);

        [[nodiscard]] mapping::category_t solve() const;
    };
}
