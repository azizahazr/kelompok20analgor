
#include <algorithm>
#include <cstdlib>
#include <utility>
#include <vector>
#include <string>
#include "nqueens.h"

namespace {

bool attacks(
    int r1,
    int c1,
    int r2,
    int c2,
    const std::vector<std::string>& grid
) {
    int dr = r2 - r1;
    int dc = c2 - c1;

    if (dr != 0 && dc != 0 && std::abs(dr) != std::abs(dc)) {
        return false;
    }

    int steps = std::max(std::abs(dr), std::abs(dc));
    int stepR = (dr > 0) - (dr < 0);
    int stepC = (dc > 0) - (dc < 0);

    for (int k = 1; k < steps; ++k) {
        int r = r1 + k * stepR;
        int c = c1 + k * stepC;

        if (grid[r][c] == 'W') {
            return false;
        }
    }

    return true;
}

void searchWalled(
    const std::vector<int>& candidates,
    const std::vector<std::vector<unsigned char>>& conflict,
    const std::vector<int>& cells,
    std::vector<int>& chosen,
    int n,
    int& bestCount,
    std::vector<std::vector<std::pair<int, int>>>& solutions
) {
    if (static_cast<int>(chosen.size() + candidates.size()) < bestCount) {
        return;
    }

    if (candidates.empty()) {
        int count = static_cast<int>(chosen.size());

        if (count > bestCount) {
            bestCount = count;
            solutions.clear();
        }

        if (count == bestCount) {
            std::vector<std::pair<int, int>> board;

            for (int index : chosen) {
                int cell = cells[index];
                board.emplace_back(cell / n, cell % n);
            }

            std::sort(board.begin(), board.end());
            solutions.push_back(std::move(board));
        }

        return;
    }

    int pick = 0;
    int maxDegree = -1;

    for (int i = 0; i < static_cast<int>(candidates.size()); ++i) {
        int degree = 0;

        for (int j = 0; j < static_cast<int>(candidates.size()); ++j) {
            if (conflict[candidates[i]][candidates[j]]) {
                ++degree;
            }
        }

        if (degree > maxDegree) {
            maxDegree = degree;
            pick = i;
        }
    }

    int vertex = candidates[pick];

    std::vector<int> includeCandidates;

    for (int candidate : candidates) {
        if (candidate != vertex && !conflict[vertex][candidate]) {
            includeCandidates.push_back(candidate);
        }
    }

    chosen.push_back(vertex);

    searchWalled(
        includeCandidates,
        conflict,
        cells,
        chosen,
        n,
        bestCount,
        solutions
    );

    chosen.pop_back();

    std::vector<int> excludeCandidates;

    for (int candidate : candidates) {
        if (candidate != vertex) {
            excludeCandidates.push_back(candidate);
        }
    }

    searchWalled(
        excludeCandidates,
        conflict,
        cells,
        chosen,
        n,
        bestCount,
        solutions
    );
}

}

std::vector<std::vector<std::pair<int, int>>> solveWalled(
    int n,
    const std::vector<std::string>& grid
) {
    std::vector<std::vector<std::pair<int, int>>> solutions;

    if (n < 1 || n > 12 || static_cast<int>(grid.size()) != n) {
        return solutions;
    }

    for (const auto& row : grid) {
        if (static_cast<int>(row.size()) != n) {
            return solutions;
        }

        for (char cell : row) {
            if (cell != '.' && cell != 'W') {
                return solutions;
            }
        }
    }

    std::vector<int> cells;

    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            if (grid[r][c] == '.') {
                cells.push_back(r * n + c);
            }
        }
    }

    int m = static_cast<int>(cells.size());

    std::vector<std::vector<unsigned char>> conflict(
        m,
        std::vector<unsigned char>(m, 0)
    );

    for (int i = 0; i < m; ++i) {
        for (int j = i + 1; j < m; ++j) {
            int r1 = cells[i] / n;
            int c1 = cells[i] % n;
            int r2 = cells[j] / n;
            int c2 = cells[j] % n;

            unsigned char value =
                attacks(r1, c1, r2, c2, grid) ? 1 : 0;

            conflict[i][j] = value;
            conflict[j][i] = value;
        }
    }

    std::vector<int> chosen;
    std::vector<int> candidates(m);

    for (int i = 0; i < m; ++i) {
        candidates[i] = i;
    }

    int bestCount = -1;

    searchWalled(
        candidates,
        conflict,
        cells,
        chosen,
        n,
        bestCount,
        solutions
    );

    return solutions;
}
