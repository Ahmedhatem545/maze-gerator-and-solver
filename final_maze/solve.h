#pragma once
#include "cell_grid.h"

// All solver function declarations
void maze_solve_dfs(Grid& grid, int startrow, int startcol, int endrow, int endcol, int windowWidth, int windowHeight);
void maze_solve_dijkstra(Grid& grid, int startRow, int startCol, int endRow, int endCol);
void maze_solve_left_wall_follower(Grid& grid, int startrow, int startcol, int endrow, int endcol);
void maze_solve_right_wall_follower(Grid& grid, int startrow, int startcol, int endrow, int endcol);