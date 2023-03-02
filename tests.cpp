#include "tests.h"

int factorial_by_mod(uint64_t n, uint64_t mod, uint64_t& res) {
    if (mod < 2) {
        return 1;
    }
    uint64_t prod = 1;
    for (uint64_t i = 1; i < n; ++i) {
        prod *= i;
        prod %= mod;
    }
    res = prod;
    return 0;
}