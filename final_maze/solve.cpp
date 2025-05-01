#include "solve.h"
#include <raylib.h>
#include <stack>
#include <queue>
#include <thread>
#include <chrono>
#include <limits>
#include <cstdlib>
#include <ctime>

const int margin = 25;

void maze_solve_dfs(Grid& grid, int startrow, int startcol, int endrow, int endcol, int windowWidth, int windowHeight) {
    std::stack<std::pair<int, int>> stack;
    grid.getCell(startrow, startcol).solved = true;
    stack.push({ startrow, startcol });
    std::srand(std::time(NULL));

    while (!stack.empty()) {
        auto current = stack.top();
        int row = current.first;
        int col = current.second;

        if (row == endrow && col == endcol) break;

        auto neighbors = grid.getNeighbors(row, col, grid.getRows(), grid.getColumns());
        std::vector<std::pair<int, int>> unvisitedNeighbors;

        for (auto& neighbor : neighbors) {
            int nr = neighbor.first;
            int nc = neighbor.second;
            if (!grid.getCell(nr, nc).solved) {
                unvisitedNeighbors.push_back(neighbor);
            }
        }

        if (!unvisitedNeighbors.empty()) {
            auto chosen = unvisitedNeighbors[std::rand() % unvisitedNeighbors.size()];
            int nr = chosen.first;
            int nc = chosen.second;

            if ((nr == row - 1 && !grid.getCell(row, col).walls[0]) ||
                (nr == row + 1 && !grid.getCell(row, col).walls[2]) ||
                (nc == col + 1 && !grid.getCell(row, col).walls[1]) ||
                (nc == col - 1 && !grid.getCell(row, col).walls[3])) {
                grid.getCell(nr, nc).solved = true;
                stack.push({ nr, nc });
            }
        }
        else {
            stack.pop();
        }

        BeginDrawing();
        ClearBackground(WHITE);
        grid.Draw(margin, margin);
        EndDrawing();
    }

    grid.getCell(endrow, endcol).solved = true;
}

void maze_solve_dijkstra(Grid& grid, int startRow, int startCol, int endRow, int endCol) {
    const int INF = std::numeric_limits<int>::max();
    int rows = grid.getRows();
    int cols = grid.getColumns();

    std::vector<std::vector<int>> dist(rows, std::vector<int>(cols, INF));
    std::vector<std::vector<std::pair<int, int>>> prev(rows, std::vector<std::pair<int, int>>(cols, { -1, -1 }));

    using pii = std::pair<int, int>;

    struct CompareDist {
        const std::vector<std::vector<int>>& dist;
        CompareDist(const std::vector<std::vector<int>>& d) : dist(d) {}
        bool operator()(const pii& a, const pii& b) const {
            return dist[a.first][a.second] > dist[b.first][b.second];
        }
    };

    std::priority_queue<pii, std::vector<pii>, CompareDist> pq((CompareDist(dist)));

    dist[startRow][startCol] = 0;
    pq.push({ startRow, startCol });

    while (!pq.empty()) {
        pii current = pq.top();
        pq.pop();
        int row = current.first;
        int col = current.second;

        if (row == endRow && col == endCol) break;

        if (grid.getCell(row, col).solved) continue;
        grid.getCell(row, col).solved = true;

        for (pii neighbor : grid.getNeighbors(row, col, rows, cols)) {
            int nr = neighbor.first;
            int nc = neighbor.second;

            bool connected =
                (nr == row - 1 && !grid.getCell(row, col).walls[0]) ||
                (nr == row + 1 && !grid.getCell(row, col).walls[2]) ||
                (nc == col + 1 && !grid.getCell(row, col).walls[1]) ||
                (nc == col - 1 && !grid.getCell(row, col).walls[3]);

            if (connected && dist[nr][nc] > dist[row][col] + 1) {
                dist[nr][nc] = dist[row][col] + 1;
                prev[nr][nc] = { row, col };
                pq.push({ nr, nc });
            }
        }

        BeginDrawing();
        ClearBackground(WHITE);
        grid.Draw(margin, margin);
        EndDrawing();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    int r = endRow;
    int c = endCol;
    while (r != -1 && c != -1) {
        grid.getCell(r, c).solved = true;
        std::tie(r, c) = prev[r][c];

        BeginDrawing();
        ClearBackground(WHITE);
        grid.Draw(margin, margin);
        EndDrawing();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void maze_solve_left_wall_follower(Grid& grid, int startrow, int startcol, int endrow, int endcol) {
    int dir = 2; // 0: up, 1: right, 2: down, 3: left
    int row = startrow;
    int col = startcol;
    grid.getCell(row, col).solved = true;

    int dRow[4] = { -1, 0, 1, 0 }; // UP, RIGHT, DOWN, LEFT
    int dCol[4] = { 0, 1, 0, -1 };

    while (!(row == endrow && col == endcol)) {
        int leftDir = (dir + 3) % 4;
        int frontDir = dir;

        if (!grid.getCell(row, col).walls[leftDir]) {
            dir = leftDir;
            row += dRow[dir];
            col += dCol[dir];
        }
        else if (!grid.getCell(row, col).walls[frontDir]) {
            row += dRow[dir];
            col += dCol[dir];
        }
        else {
            dir = (dir + 1) % 4;
        }

        if (row < 0 || row >= grid.getRows() || col < 0 || col >= grid.getColumns()) break;

        grid.getCell(row, col).solved = true;

        BeginDrawing();
        ClearBackground(WHITE);
        grid.Draw(margin, margin);
        EndDrawing();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
