#include <iostream>
#include <ranges>
#include <unordered_map>
#include <algorithm>
#include <sstream>

using std::operator ""sv;

int main() {
    freopen("./input.txt", "r", stdin);

    static const std::unordered_map<std::string_view, int> max_cubes{{"red"sv,   12},
                                                                     {"green"sv, 13},
                                                                     {"blue"sv,  14}};

    std::string games;
    std::ranges::copy(std::ranges::views::istream<std::string>(std::cin)
                      | std::ranges::views::join_with(' '), std::back_inserter(games));

    auto legit_games = std::string_view(games)
                       | std::ranges::views::split("Game"sv)
                       | std::ranges::views::enumerate
                       | std::ranges::views::filter([](auto const &enum_game) -> bool {
        auto game = std::get<1>(enum_game) | std::ranges::views::drop_while([](char ch) {
            return ch != ':';
        }) | std::ranges::views::drop(1);

        auto drafts = std::ranges::views::split(std::string_view(game), ";"sv);

        return std::ranges::all_of(drafts, [](auto const &draft) -> bool {
            auto cube_drafts = std::ranges::views::split(std::string_view(draft), ","sv);
            return std::ranges::all_of(cube_drafts, [](auto const &cube_draft) -> bool {
                std::stringstream ss;
                ss << std::string_view(cube_draft);
                int cube_count;
                std::string colour;
                ss >> cube_count >> colour;
                return cube_count <= max_cubes.find(colour)->second;
            });
        });
    }) | std::ranges::views::keys;

    std::cout << std::ranges::fold_left(legit_games, 0, std::plus()) << std::endl;

    return 0;
}