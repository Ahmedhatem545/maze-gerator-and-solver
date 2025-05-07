#include "gen.h"
#include <raylib.h>
#include <stack>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <random>
#include<algorithm>
#include <tuple>
using namespace std;

const int margin = 25;

void generateMaze_dfs(Grid& grid, int startRow, int startCol, int endRow, int endCol) {
    std::stack<std::pair<int, int>> stack;
    grid.getCell(startRow, startCol).visited = true;
    stack.push({ startRow, startCol });
    std::srand(std::time(NULL));

    grid.getCell(endRow, endCol).walls[2] = 0;
    grid.getCell(startRow, startCol).walls[0] = 0;

    while (!stack.empty()) {
        auto current = stack.top();
        int row = current.first;
        int col = current.second;

        auto neighbors = grid.getNeighbors(row, col, grid.getRows(), grid.getColumns());
        std::vector<std::pair<int, int>> unvisitedNeighbors;

        for (auto& neighbor : neighbors) {
            int nr = neighbor.first;
            int nc = neighbor.second;
            if (!grid.getCell(nr, nc).visited) {
                unvisitedNeighbors.push_back(neighbor);
            }
        }

        if (!unvisitedNeighbors.empty()) {
            auto chosen = unvisitedNeighbors[std::rand() % unvisitedNeighbors.size()];
            int nr = chosen.first;
            int nc = chosen.second;

            grid.removeWall(row, col, nr, nc);
            grid.getCell(nr, nc).visited = true;
            stack.push({ nr, nc });
        }
        else {
            stack.pop();
        }

        BeginDrawing();
        ClearBackground(WHITE);
        grid.Draw(margin, margin);
        EndDrawing();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    grid.getCell(endRow, endCol).visited = true;
    grid.getCell(endRow, endCol).walls[2] = 0;
    grid.getCell(startRow, startCol).walls[0] = 0;
}



void generateMaze_prim(Grid& grid, int startRow, int startCol, int endRow, int endCol) {
    std::srand(std::time(NULL));
    int rows = grid.getRows();
    int cols = grid.getColumns();

    using pii = std::pair<int, int>;
    std::vector<pii> frontier;

    auto inBounds = [&](int r, int c) {
        return r >= 0 && r < rows && c >= 0 && c < cols;
        };

    auto addFrontier = [&](int r, int c) {
        if (inBounds(r, c) && !grid.getCell(r, c).visited) {
            grid.getCell(r, c).visited = true;
            frontier.push_back({ r, c });
        }
        };

    // Start with a single cell
    grid.getCell(startRow, startCol).visited = true;

    // Add neighbors to frontier
    addFrontier(startRow - 1, startCol);
    addFrontier(startRow + 1, startCol);
    addFrontier(startRow, startCol - 1);
    addFrontier(startRow, startCol + 1);

    while (!frontier.empty()) {
        int idx = std::rand() % frontier.size();
        pii current = frontier[idx];
        int r = current.first;
        int c = current.second;
        frontier.erase(frontier.begin() + idx);

        std::vector<pii> neighbors = {
            {r - 1, c},
            {r + 1, c},
            {r, c - 1},
            {r, c + 1}
        };

        std::vector<pii> visitedNeighbors;
        for (const auto& neighbor : neighbors) {
            int nr = neighbor.first;
            int nc = neighbor.second;
            if (inBounds(nr, nc) && grid.getCell(nr, nc).visited) {
                bool inFrontier = false;
                for (const auto& f : frontier) {
                    if (f.first == nr && f.second == nc) {
                        inFrontier = true;
                        break;
                    }
                }
                if (!inFrontier) {
                    visitedNeighbors.push_back({ nr, nc });
                }
            }
        }

        if (!visitedNeighbors.empty()) {
            int randIdx = std::rand() % visitedNeighbors.size();
            int nr = visitedNeighbors[randIdx].first;
            int nc = visitedNeighbors[randIdx].second;
            grid.removeWall(r, c, nr, nc);
        }

        grid.getCell(r, c).visited = true;

        // Add new frontier cells
        addFrontier(r - 1, c);
        addFrontier(r + 1, c);
        addFrontier(r, c - 1);
        addFrontier(r, c + 1);
        BeginDrawing();
        ClearBackground(WHITE);
        grid.Draw(margin, margin);
        EndDrawing();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // Remove entrance and exit walls
    grid.getCell(startRow, startCol).walls[0] = 0;
    grid.getCell(endRow, endCol).walls[2] = 0;
}

void generateMaze_binaryTree(Grid& grid, int startRow, int startCol, int endRow, int endCol)
{
    srand(time(NULL));
    for (int row = grid.getRows()-1;row >=0;row--) {
        for (int col = 0;col < grid.getColumns();col++) {
            grid.getCell(row, col).visited = true;
            vector<pair<int, int>> neighbors;
            if (row > 0) {
                neighbors.emplace_back(row - 1, col);
            }
            if (col < grid.getColumns() - 1) {
                neighbors.emplace_back(row, col + 1);
            }
            if (!neighbors.empty()) {
                pair<int, int> chosen = neighbors[rand() % neighbors.size()];
                int nrow = chosen.first;
                int ncol = chosen.second;
                grid.removeWall(row, col, nrow, ncol);
            }
            BeginDrawing();
            ClearBackground(WHITE);
            grid.Draw(margin, margin);
            EndDrawing();
            this_thread::sleep_for(chrono::milliseconds(1));
        }
    }
    grid.getCell(startRow, startCol).walls[0] = false;
    grid.getCell(endRow, endCol).walls[2] = false;
}




