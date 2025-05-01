#include "game_loop.h"

void gameLoop()
{
    const int windowWidth = 800;
    const int windowHeight = 700;
    const int cellsize = 25;
    const int fps = 100;
    bool solved = false;
    bool generated = false;

    InitWindow(windowWidth, windowHeight, "Maze Generator (DFS)");
    SetTargetFPS(fps);

    Grid grid(windowWidth - 2 * margin, windowHeight - 2 * margin, cellsize);

    int startRow = 0;
    int startCol = 0;
    int endRow = grid.getRows() - 1;
    int endCol = grid.getColumns() - 1;

    Texture2D background = LoadTexture("graphics/maze_gen_background.jpg");

    // Button layout
    float centerX = windowWidth / 2.0f - 90; // Adjusted for 0.9 scale
    float startY = 250;
    float spacingY = 60;

    button dfs_gen_button("graphics/button_start.png", { centerX, startY + spacingY * 0 }, 0.9f);
    button gen_prims_button("graphics/button_gen-prims.png", { centerX,startY + spacingY * 1 }, 0.9f);
    button solve_dfs_button("graphics/button_dfs.png", { centerX, startY + spacingY * 2 }, 0.8f);
    button dijk_button("graphics/button_dijkstar.png", { centerX, startY + spacingY * 3 }, 0.8f);
    button leftWall_button("graphics/button_left-wall.png", { centerX, startY + spacingY * 4 }, 0.8f);
    button reset_button("graphics/button_reset.png", { centerX, startY + spacingY * 5 }, 0.8f);

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        DrawTexture(background, 0, 0, WHITE);

        if (reset_button.is_pressed(mousePos, mousePressed)) {
            grid.Reset();
            solved = false;
            generated = false;
        }

        if (dfs_gen_button.is_pressed(mousePos, mousePressed)) {
            generateMaze_dfs(grid, startRow, startCol, endRow, endCol);
            generated = true;
        }
        if (gen_prims_button.is_pressed(mousePos, mousePressed)) {
            generateMaze_prim(grid, startRow, startCol, endRow, endCol);
            generated = true;
        }

        if (solve_dfs_button.is_pressed(mousePos, mousePressed)) {
            maze_solve_dfs(grid, startRow, startCol, endRow, endCol, windowWidth, windowHeight);
            solved = true;
            generated = false;
        }

        if (dijk_button.is_pressed(mousePos, mousePressed)) {
            maze_solve_dijkstra(grid, startRow, startCol, endRow, endCol);
            solved = true;
            generated = false;
        }

        if (leftWall_button.is_pressed(mousePos, mousePressed)) {
            maze_solve_left_wall_follower(grid, startRow, startCol, endRow, endCol);
            solved = true;
            generated = false;
        }

        BeginDrawing();
        ClearBackground(WHITE);
        grid.Draw(margin, margin);

        if (!generated && !solved) {
            dfs_gen_button.Draw();
            gen_prims_button.Draw();
        }

        if (solved) {
            reset_button.Draw();
        }

        if (generated) {
            solve_dfs_button.Draw();
            dijk_button.Draw();
            leftWall_button.Draw();
            reset_button.Draw();  // Optional: reset can also show here
        }

        EndDrawing();
    }

    CloseWindow();
}
