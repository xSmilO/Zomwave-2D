#pragma once
#include "Animator.h"
#include "Entities/Enemy.h"

class Zombie : public Enemy {
  private:
    float speed;
    bool texFlip;
    Animator animator;

  public:
    Zombie(Texture2D *texZombie, Vector2 startPos);

    void Update(float dt, Vector2 playerPos, Map *map) override;
    void Draw() override;
    Rectangle GetHitbox() override;
};
