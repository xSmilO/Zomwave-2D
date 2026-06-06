#pragma once

#include "Animator.h"
#include "Entities/Enemy.h"
#include "Managers/AudioManager.h"
#include "Managers/BulletManager.h"
#include "Map.h"
#include "raylib.h"

class Boss : public Enemy {
  private:
    float fireRate = 2.0f;
    float fireTimer = 0.0f;
    float attackRange = 250.0f;
    float stopRange = 150.0f;
    Animator animator;
    bool texFlip;

    BulletManager *bulletManager;

  public:
    Boss(Texture2D *texCharacter, Texture2D *texBow, Vector2 startPos,
         BulletManager *bm, AudioManager *am);

    void Update(float dt, Vector2 playerPos, Map *map) override;
    void Draw() override;
    Rectangle GetHitbox() override;
};
