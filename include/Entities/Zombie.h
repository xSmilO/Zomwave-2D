#pragma once
#include "Animator.h"
#include "Entities/Enemy.h"

class Zombie : public Enemy {
  private:
    float speed;
    bool texFlip;
    Animator animator;
    float ambientTimer;

  public:
    Zombie(Texture2D *texZombie, Vector2 startPos, AudioManager *am);

    void Update(float dt, Vector2 playerPos, Map *map) override;
    void Draw() override;
    Rectangle GetHitbox() override;
};
