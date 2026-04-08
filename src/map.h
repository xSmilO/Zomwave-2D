#pragma once
#include "raylib.h"

const int TILE_SIZE = 64;
const int MAP_COLS = 15;
const int MAP_ROWS = 10;

class GameMap {
  private:
  int grid[MAP_ROWS][MAP_COLS];
public:
  GameMap();
  void Draw();
};
