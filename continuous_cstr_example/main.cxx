#include <ranges>
#include <string>
#include <map>
#include <range/v3/to_container.hpp>
#include <charconv>
#include <iostream>

using namespace std::literals;

struct draft {
    std::map<std::string, int> count_;

    friend std::ostream &operator<<(std::ostream &os, draft const &draft_) {
        return os << (draft_.count_ | std::views::transform([](auto &&pp) {
            return pp.first + ": " + std::to_string(pp.second);
        })
                      | std::views::join_with(", "sv)
                      | ranges::to<std::string>()) << std::endl;
    }
};

int main() {
    constexpr auto s1 = "red 1 blue 2, red 4 yellow 3"sv;

    const auto drafts = std::views::split(s1, ", "sv)
        | std::views::transform([](auto &&draft_str) -> draft {
        return draft{
                .count_ = draft_str | std::views::split(" "sv) | std::views::chunk(2)
                          | std::views::transform([](auto &&chunk) -> std::pair<std::string, int> {
                    auto iter = chunk.begin();
                    const auto colour_iter = iter++;
                    std::string colour = *colour_iter | ranges::to<std::string>();

                    const auto count_iter = iter++;
                    auto count_data = std::ranges::data(*count_iter);
                    auto count_length = std::ranges::size(*count_iter);
                    int count;
                    std::from_chars(count_data, count_data+count_length, count);

                    return {colour, count};
                }) | ranges::to<std::map>()
        };
    }) | ranges::to<std::vector>();

    for(auto& draft_: drafts) {
        std::cout << draft_ << std::endl;
    }
}