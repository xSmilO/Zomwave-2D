#pragma once
#include "Map.h"
#include "raylib.h"

class Enemy {
public:
    Vector2 position;
    float width, height;
    float health;
    bool active = true;

    virtual ~Enemy() = default;

    virtual void Update(Vector2 playerPos, Map* map) = 0;
    virtual void Draw() = 0;
    virtual Rectangle GetHitbox() = 0;
};
