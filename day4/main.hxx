#include <ranges>
#include <list>

namespace aoc::day3 {
    struct card {
        std::list<int> winnings_;
        std::list<int> content_;

        [[nodiscard]] std::size_t count() const;

        using score_t = int;

        [[nodiscard]] score_t score() const;
    };

    std::list<card> read_inputs(std::istream &);
}
