#pragma once

#include <LDtkLoader/Project.hpp>
#include "raylib.h"
#include <vector>


class Map {
  private:
    ldtk::Project proj;
    const ldtk::Layer* collisionLayer;
    Vector2 spawnPoint;
    Texture2D layerTex;
    int mapWidth;
    int mapHeight;
    int tileSize;
    std::vector<ldtk::Tile> tiles;
    void LoadMap();

  public:
    Map();
    ~Map();
    void Draw();
    Vector2 GetSpawnPoint();
    bool CheckHitbox(Rectangle hitbox);
};
