#pragma once
#include "Animator.h"
#include "Managers/BulletManager.h"

class Pistol {
  private:
    Rectangle position;
    double lastTimeShoot;
    float fireRate;
    float fireTimer;
    float _rotation;
    Animator animator;

  public:
    float width;
    float height;
    Pistol(Texture2D* texPistol);
    void Update(float dt, Vector2 newPosition, float rotation);
    void Shoot(Vector2 targetPos, BulletManager *bulletManager);
    void Draw();
};
