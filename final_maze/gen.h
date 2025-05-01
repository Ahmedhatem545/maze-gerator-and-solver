#pragma once
#include "cell_grid.h"

// No function body here — just declaration
void generateMaze_dfs(Grid& grid, int startRow, int startCol, int endRow, int endCol);
void generateMaze_prim(Grid& grid, int startRow, int startCol, int endRow, int endCol);

