#pragma once
#include "Animator.h"

class Pistol {
  private:
    double lastTimeShoot;
    double fireRate;
    Animator animator;

  public:
    Pistol();
    void Update();
    void Draw(Vector2 position, float rotation);
};
