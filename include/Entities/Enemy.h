#pragma once
#include "Managers/AudioManager.h"
#include "Map.h"
#include "raylib.h"

class Enemy {
  protected:
    float width, height;
    AudioManager* audioManager;

  public:
    Vector2 position;
    bool active = true;
    float health;
    float maxHealth;
    float attackCooldown = 1.0f;
    float attackTimer = 0.0f;
    int damage = 15;


    virtual ~Enemy() = default;

    virtual void Update(float dt, Vector2 playerPos, Map *map) = 0;
    virtual void Draw() = 0;
    virtual Rectangle GetHitbox() = 0;

    void DrawHealthBar();
};
