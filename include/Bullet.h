#pragma once

#include "Map.h"
#include "raylib.h"
class Bullet {
  public:
    Vector2 position;
    Vector2 direction;
    float speed;
    float radius;
    bool active;

    Bullet(Vector2 startPos, Vector2 targetPos, float bulletSpeed);

    void Update(Map* map);
    void Draw();
};
