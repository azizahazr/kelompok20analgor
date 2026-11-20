
#include <vector>
#include "nqueens.h"

namespace {

void searchPreplaced(
    int n,
    int row,
    unsigned int full,
    unsigned int columns,
    unsigned int diagLeft,
    unsigned int diagRight,
    const std::vector<int>& fixed,
    std::vector<int>& current,
    std::vector<std::vector<int>>& solutions
) {
    if (row == n) {
        solutions.push_back(current);
        return;
    }

    unsigned int available =
        full & ~(columns | diagLeft | diagRight);

    if (fixed[row] != -1) {
        available &= (1U << fixed[row]);
    }

    while (available != 0) {
        unsigned int bit = available & (~available + 1U);
        available ^= bit;

        int col = 0;
        while (((bit >> col) & 1U) == 0U) {
            ++col;
        }

        current[row] = col;

        searchPreplaced(
            n,
            row + 1,
            full,
            columns | bit,
            ((diagLeft | bit) << 1) & full,
            (diagRight | bit) >> 1,
            fixed,
            current,
            solutions
        );
    }
}

}

std::vector<std::vector<int>> solvePreplaced(
    int n,
    const std::vector<std::pair<int, int>>& preplaced
) {
    std::vector<std::vector<int>> solutions;

    if (n < 1 || n > 12) {
        return solutions;
    }

    std::vector<int> fixed(n, -1);

    for (const auto& queen : preplaced) {
        int row = queen.first;
        int col = queen.second;

        if (row < 0 || row >= n || col < 0 || col >= n) {
            return solutions;
        }

        if (fixed[row] != -1 && fixed[row] != col) {
            return solutions;
        }

        fixed[row] = col;
    }

    unsigned int full = (1U << n) - 1U;
    std::vector<int> current(n, -1);

    searchPreplaced(
        n,
        0,
        full,
        0,
        0,
        0,
        fixed,
        current,
        solutions
    );

    return solutions;
}
