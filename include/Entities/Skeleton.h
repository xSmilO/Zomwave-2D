#pragma once

#include "Animator.h"
#include "Entities/Enemy.h"
#include "Managers/BulletManager.h"
#include "Map.h"
#include "raylib.h"
class Skeleton : public Enemy {
  private:
    Vector2 position;
    float speed = 80.0f;
    float fireRate = 2.0f;
    float fireTimer = 0.0f;
    float attackRange = 200.0f;
    float stopRange = 150.0f;
    float bowRotation;
    float bowWidth = 16.0f;
    float bowHeight = 16.0f;
    Animator animator;
    bool texFlip;

    Animator bowAnimator;
    Vector2 bowPosition;

    BulletManager *bulletManager;

  public:
    Skeleton(Texture2D *texCharacter, Texture2D* texBow, Vector2 startPos, BulletManager *bm);

    void CalculateBowPos(Vector2 targetPos);

    void Update(float dt, Vector2 playerPos, Map *map) override;
    void Draw() override;
    Rectangle GetHitbox() override;
};
