#include <algorithm>
#include <iostream>
#include "main.hxx"
#include <range/v3/to_container.hpp>
#include <sstream>
#include "commons.hxx"

using std::operator ""sv;

namespace aoc::day4 {

    std::size_t card::count() const {
        return std::ranges::count_if(content_, [this](int value) {
            return std::ranges::contains(winnings_, value);
        });
    }

    card::score_t card::score() const {
        auto count_ = count();
        return count_ ? 1 << (count_ - 1) : 0;
    }

    static constexpr auto number_chunk = [](char prev, char cur) {
        return isdigit(prev) == isdigit(cur);
    };

    std::list<card> read_inputs(std::istream &is) {
        auto cards = std::views::istream<CompleteLine>(is) | std::views::transform([](std::string line) {
            auto winnings_and_content = line | std::views::drop_while([](char ch) {
                return ch != ':';
            }) | std::views::drop(1) | std::views::split('|');
            auto winnings = winnings_and_content.front() | std::views::chunk_by(number_chunk) |
                            std::views::filter([](auto &&chunk) {
                                return isdigit(chunk.front());
                            }) | std::views::transform([](auto &&chunk) {
                return stoi(chunk | ranges::to<std::string>());
            }) | ranges::to<std::list>();
            auto content = *(++winnings_and_content.begin()) | std::views::chunk_by(number_chunk) |
                           std::views::filter([](auto &&chunk) {
                               return isdigit(chunk.front());
                           }) | std::views::transform([](auto &&chunk) {
                return stoi(chunk | ranges::to<std::string>());
            }) | ranges::to<std::list>();
            return card{.winnings_ = winnings, .content_ = content};
        });

        std::list<card> owning;
        std::ranges::copy(cards, std::back_inserter(owning));
        return owning;
    }
}

int main() {
    using namespace aoc::day4;

    freopen("./input.txt", "r", stdin);

    auto cards = read_inputs(std::cin);
    auto scores = cards | std::views::transform([](auto &&card) {
        return card.score();
    });
    auto total_score = std::ranges::fold_left(scores, 0, std::plus());

    std::cout << total_score << std::endl;

    auto num_cards = cards.size();
    std::vector<int> won_cards(num_cards, 1);

    for (const auto &[idx, card_]: cards | std::views::enumerate) {
        auto &won = won_cards[idx];
        auto count_ = card_.count();
        for (std::size_t j = 1; j <= count_ && idx + j < num_cards; j++) {
            won_cards[idx+j] += won;
        }
    }

    std::cout << std::ranges::fold_left(won_cards, 0, std::plus()) << std::endl;
}
