#pragma once
#include "Entities/Player.h"
#include "Managers/AudioManager.h"
#include "Managers/EnemyManager.h"
#include <raylib.h>
#include <raymath.h>
#include <vector>

struct Bomb {
    Vector2 startPos;
    Vector2 currentPos;
    Vector2 targetPos;
    float flightTimer;
    float flightDuration;

    bool isExploding = false;
    float explosionTimer = 0.0f;
};

class BombManager {
  public:
    std::vector<Bomb> bombs;

    float explosionRadius = 200.0f;
    float damage = 250.0f;

    void ThrowBomb(Vector2 start, Vector2 target);
    void Update(float dt, Player* player, EnemyManager *enemyManager,
                AudioManager *audioManager);
    void Draw(Texture2D texBomb);
};
