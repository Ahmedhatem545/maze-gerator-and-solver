#pragma once
#include <raylib.h>
#include <vector>

struct Cell {
    int x, y;
    bool walls[4];
    bool visited = false;
    bool solved = false;

    Cell(int _x, int _y);

    void draw(int cellsize, int xOffset, int yOffset) const;
};

class Grid {
public:
    Grid(int width, int height, int cellsize);
    void Draw(int offsetx, int offsety);
    void removeWall(int row1, int col1, int row2, int col2);
    Cell& getCell(int row, int col);
    int getRows() const;
    int getColumns() const;
    std::vector<std::vector<Cell>> getcells();
    void Reset();
    std::vector<std::pair<int, int>> getNeighbors(int row, int col, int rows, int columns);

private:
    int rows;
    int columns;
    int cellSize;
    std::vector<std::vector<Cell>> cells;
};
