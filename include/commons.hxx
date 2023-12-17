#include <istream>

namespace aoc {
    struct CompleteLine {    // Line Proxy for the input Iterator
        friend std::istream& operator>>(std::istream& is, CompleteLine& cl) { std::getline(is, cl.completeLine); return is; }
        operator std::string() const { return completeLine; }  // cast operator
        std::string completeLine{};
    };
}
