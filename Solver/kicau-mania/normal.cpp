
#include <cstdint>
#include <vector>
#include "nqueens.h"

namespace {

void searchQueens(
    int n,
    int row,
    unsigned int full,
    unsigned int columns,
    unsigned int diagLeft,
    unsigned int diagRight,
    std::vector<int>& current,
    std::vector<std::vector<int>>& solutions
) {
    if (row == n) {
        solutions.push_back(current);
        return;
    }

    unsigned int available =
        full & ~(columns | diagLeft | diagRight);

    while (available != 0) {
        unsigned int bit = available & (~available + 1U);
        available ^= bit;

        int col = 0;
        while (((bit >> col) & 1U) == 0U) {
            ++col;
        }

        current[row] = col;

        searchQueens(
            n,
            row + 1,
            full,
            columns | bit,
            ((diagLeft | bit) << 1) & full,
            (diagRight | bit) >> 1,
            current,
            solutions
        );
    }
}

}

std::vector<std::vector<int>> solveNQueens(int n) {
    std::vector<std::vector<int>> solutions;

    if (n < 1 || n > 12) {
        return solutions;
    }

    unsigned int full = (1U << n) - 1U;

    std::vector<int> current(n, -1);

    searchQueens(
        n,
        0,
        full,
        0,
        0,
        0,
        current,
        solutions
    );

    return solutions;
}
