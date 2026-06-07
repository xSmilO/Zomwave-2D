#pragma once

#include "Animator.h"
#include "Entities/Enemy.h"
#include "Managers/AudioManager.h"
#include "Managers/BulletManager.h"
#include "Map.h"
#include "raylib.h"

class Boss : public Enemy {
  private:
    float attackRange = 200.0f;
    float stopRange = 150.0f;
    float ambientTimer;
    Animator animator;
    bool texFlip;

    BulletManager *bulletManager;

  public:
    Boss(Texture2D *texIdle, Texture2D *texWalk, Texture2D *texAttack,
         Vector2 startPos, BulletManager *bm, AudioManager *am);

    void Update(float dt, Vector2 playerPos, Map *map) override;
    void Draw() override;
    Rectangle GetHitbox() override;
    void DrawHealthBar() override;
};
