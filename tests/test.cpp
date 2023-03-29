#include <cstdint>
#include <iostream>

int factorial_by_mod(uint64_t n, uint64_t mod, uint64_t& res) {
    if (mod < 2) {
        return 1;
    }
    uint64_t prod = 1;
    for (uint64_t i = 1; i < n + 1; ++i) {
        prod *= i;
        prod %= mod;
    }
    res = prod;
    return 0;
}

int main() {
    uint64_t n, mod, res, count;
    std::cin >> mod >> count;
    for (uint64_t i = 0; i < count; ++i) {
         std::cin >> n;
         int tmp = 0;
         if ((tmp = factorial_by_mod(n, mod, res))) {
             return tmp;
         }
         std::cout << res << std::endl;
    }
    return 0;
}
