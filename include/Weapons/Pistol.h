#pragma once
#include "Animator.h"
#include "Managers/BulletManager.h"

class Pistol {
  private:
    Rectangle _position;
    double lastTimeShoot;
    float fireRate;
    float fireTimer;
    float _rotation;
    Animator animator;

  public:
    Pistol(Texture2D* pistolTex);
    void Update(Vector2 position, float rotation, float dt);
    void Shoot(Vector2 targetPos, BulletManager *bulletManager);
    void Draw();
};
