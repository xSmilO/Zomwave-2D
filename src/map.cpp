#include "map.h"

GameMap::GameMap() {
    int initialMap[MAP_ROWS][MAP_COLS] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

    for (int y = 0; y < MAP_ROWS; ++y) {
        for (int x = 0; x < MAP_COLS; ++x)
            grid[y][x] = initialMap[y][x];
    }
}

void GameMap::Draw() {
    for (int y = 0; y < MAP_ROWS; ++y) {
        for (int x = 0; x < MAP_COLS; ++x) {
            int worldX = x * TILE_SIZE;
            int worldY = y * TILE_SIZE;

            if (grid[y][x] == 1) {
                DrawRectangle(worldX, worldY, TILE_SIZE, TILE_SIZE, DARKGRAY);
                DrawRectangleLines(worldX, worldY, TILE_SIZE, TILE_SIZE, BLACK);
            } else if (grid[y][x] == 0) {
                DrawRectangle(worldX, worldY, TILE_SIZE, TILE_SIZE, LIGHTGRAY);
                DrawRectangleLines(worldX, worldY, TILE_SIZE, TILE_SIZE, GRAY);
            }
        }
    }
}
