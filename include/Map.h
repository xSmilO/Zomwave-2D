#pragma once

#include "raylib.h"
#include <LDtkLoader/Project.hpp>
#include <map>

class Map {
  private:
    ldtk::Project project;
    const ldtk::Level* currentLevel;
    const ldtk::Layer *collisionLayer = nullptr;
    float tileSize = 16.0f;
    Vector2 mapSize;

    std::map<std::string, Texture2D> tilesets;

    void DrawLayer(const ldtk::Layer& layer);
  public:
    Map();
    ~Map();
    Vector2 GetSpawnPoint();
    bool CheckHitbox(Rectangle hitbox);
    void DrawBackground();
    void DrawForeground();
};
