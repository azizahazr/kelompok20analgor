
#include <vector>
#include <string>
#include "nqueens.h"

namespace {

void searchHoles(
    int n,
    int row,
    unsigned int full,
    unsigned int columns,
    unsigned int diagLeft,
    unsigned int diagRight,
    const std::vector<std::string>& grid,
    std::vector<int>& current,
    std::vector<std::vector<int>>& solutions
) {
    if (row == n) {
        solutions.push_back(current);
        return;
    }

    unsigned int available =
        full & ~(columns | diagLeft | diagRight);

    for (int col = 0; col < n; ++col) {
        if (grid[row][col] == 'H') {
            available &= ~(1U << col);
        }
    }

    while (available != 0) {
        unsigned int bit = available & (~available + 1U);
        available ^= bit;

        int col = 0;
        while (((bit >> col) & 1U) == 0U) {
            ++col;
        }

        current[row] = col;

        searchHoles(
            n,
            row + 1,
            full,
            columns | bit,
            ((diagLeft | bit) << 1) & full,
            (diagRight | bit) >> 1,
            grid,
            current,
            solutions
        );
    }
}

}

std::vector<std::vector<int>> solveHoles(
    int n,
    const std::vector<std::string>& grid
) {
    std::vector<std::vector<int>> solutions;

    if (n < 1 || n > 12 || static_cast<int>(grid.size()) != n) {
        return solutions;
    }

    for (const auto& row : grid) {
        if (static_cast<int>(row.size()) != n) {
            return solutions;
        }
    }

    unsigned int full = (1U << n) - 1U;
    std::vector<int> current(n, -1);

    searchHoles(
        n,
        0,
        full,
        0,
        0,
        0,
        grid,
        current,
        solutions
    );

    return solutions;
}
