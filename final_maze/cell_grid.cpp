#pragma once
#include "cell_grid.h"
Cell::Cell(int _x, int _y) {
    x = _x;
    y = _y;
    walls[0] = walls[1] = walls[2] = walls[3] = true;
}

void Cell::draw(int cellsize, int xOffset, int yOffset) const {
    int px = x * cellsize + xOffset;
    int py = y * cellsize + yOffset;

    if (visited) {
        DrawRectangle(px, py, cellsize, cellsize, SKYBLUE);
    }
    if (solved) {
        DrawRectangle(px, py, cellsize, cellsize, GREEN);
    }
    if (walls[0]) DrawLine(px, py, px + cellsize, py, BLACK);         // Top
    if (walls[1]) DrawLine(px + cellsize, py, px + cellsize, py + cellsize, BLACK); // Right
    if (walls[2]) DrawLine(px + cellsize, py + cellsize, px, py + cellsize, BLACK); // Bottom
    if (walls[3]) DrawLine(px, py + cellsize, px, py, BLACK);         // Left
}

// GRID IMPLEMENTATION
Grid::Grid(int width, int height, int cellsize)
    : rows(height / cellsize), columns(width / cellsize), cellSize(cellsize) {
    for (int y = 0; y < rows; y++) {
        std::vector<Cell> row;
        for (int x = 0; x < columns; x++) {
            row.emplace_back(x, y);
        }
        cells.push_back(row);
    }
}

void Grid::Draw(int offsetx, int offsety) {
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            cells[y][x].draw(cellSize, offsetx, offsety);
        }
    }
}

void Grid::removeWall(int row1, int col1, int row2, int col2) {
    if (row1 == row2) {
        if (col1 < col2) {
            cells[row1][col1].walls[1] = false;
            cells[row2][col2].walls[3] = false;
        }
        else if (col1 > col2) {
            cells[row1][col1].walls[3] = false;
            cells[row2][col2].walls[1] = false;
        }
    }
    else if (col1 == col2) {
        if (row1 < row2) {
            cells[row1][col1].walls[2] = false;
            cells[row2][col2].walls[0] = false;
        }
        else if (row1 > row2) {
            cells[row1][col1].walls[0] = false;
            cells[row2][col2].walls[2] = false;
        }
    }
}

Cell& Grid::getCell(int row, int col) {
    return cells[row][col];
}

int Grid::getRows() const {
    return rows;
}

int Grid::getColumns() const {
    return columns;
}

std::vector<std::vector<Cell>> Grid::getcells() {
    return cells;
}

void Grid::Reset() {
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            cells[y][x].visited = false;
            cells[y][x].solved = false;
            cells[y][x].walls[0] = true;
            cells[y][x].walls[1] = true;
            cells[y][x].walls[2] = true;
            cells[y][x].walls[3] = true;
        }
    }
}

std::vector<std::pair<int, int>> Grid::getNeighbors(int row, int col, int rows, int columns) {
    std::vector<std::pair<int, int>> neighbors;
    if (row > 0) neighbors.push_back({ row - 1, col });
    if (col < columns - 1) neighbors.push_back({ row, col + 1 });
    if (row < rows - 1) neighbors.push_back({ row + 1, col });
    if (col > 0) neighbors.push_back({ row, col - 1 });
    return neighbors;
}