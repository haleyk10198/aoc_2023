#include <iostream>
#include <ranges>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <range/v3/to_container.hpp>

using std::operator ""sv;

struct Draft {
    std::string colour_;
    int count_;

    std::weak_ordering operator<=>(Draft const&) const = default;
};

int main() {
    freopen("./input.txt", "r", stdin);

    std::string lines;
    std::ranges::copy(std::ranges::views::istream<std::string>(std::cin)
                      | std::ranges::views::join_with(' '), std::back_inserter(lines));

    auto game_results = std::string_view(lines)
                        | std::ranges::views::split("Game"sv)
                        | std::views::drop(1)
                        | std::ranges::views::transform([](auto const &line) -> int {
        auto game = line | std::ranges::views::drop_while([](char ch) {
            return ch != ':';
        }) | std::ranges::views::drop(1);

        auto drafts = std::ranges::views::split(std::string_view(game), ";"sv);

        auto cube_drafts = drafts | std::ranges::views::transform([](auto const &draft) {
            return std::string_view(draft) | std::views::split(","sv)
                   | std::ranges::views::transform([](auto const &cube_draft) {
                std::stringstream ss;
                ss << std::string_view(cube_draft);
                int cube_count;
                std::string colour;
                ss >> cube_count >> colour;
                return Draft{.colour_ = colour, .count_ = cube_count};
            });
        }) | std::views::join | ranges::to<std::vector>();

        std::ranges::sort(cube_drafts);

        auto coloured_drafts = std::ranges::views::chunk_by(cube_drafts, [](const Draft &lhs, const Draft& rhs) {
            return lhs.colour_ == rhs.colour_;
        });
        auto coloured_bests = coloured_drafts | std::ranges::views::transform([](auto const& group) {
            std::cout << group.back().colour_ << ": " << group.back().count_ << std::endl;
            return group.back().count_;
        });

        std::cout << std::ranges::fold_left(coloured_bests, 1, std::multiplies()) << std::endl;

        return std::ranges::fold_left(coloured_bests, 1, std::multiplies());
    });

    std::cout << std::ranges::fold_left(game_results, 0, std::plus()) << std::endl;

    return 0;
}