#include "Weapons/Pistol.h"
#include "Managers/BulletManager.h"
#include "raylib.h"
#include "raymath.h"

Pistol::Pistol(Texture2D *texPistol) {

    std::vector<Vector2> shootFramePos = {{0, 0}, {1, 0}, {2, 0},  {3, 0},
                                          {4, 0}, {5, 0}, {6, 0},  {7, 0},
                                          {8, 0}, {9, 0}, {10, 0}, {11, 0}};

    animator.AddAnimation("idle", texPistol, {64, 32}, 0, {{0, 0}}, false);
    animator.AddAnimation("shoot", texPistol, {64, 32}, 38, shootFramePos,
                          false);

    animator.SetState("idle");
    fireRate = 0.3;
    fireTimer = fireRate;
    width = 35;
    height = 25;
}

void Pistol::Update(float dt, Vector2 newPosition, float rotation) {
    position = {newPosition.x, newPosition.y, width, height};
    _rotation = rotation;
    animator.Update(dt);
    fireTimer += dt;
}

void Pistol::Draw() {
    animator.Draw(position, false, _rotation);
    // DrawRectangleRec(_position, RED);
}

void Pistol::Shoot(Vector2 targetPos, BulletManager *bulletManager) {
    // if (fire)
    if (fireTimer >= fireRate) {
        Vector2 direction =
            Vector2Subtract(targetPos, {position.x, position.y});
        direction = Vector2Normalize(direction);

        bulletManager->SpawnBullet({position.x, position.y}, targetPos, 800);
        animator.SetState("shoot");
        animator.ResetAnimation();

        fireTimer = 0.0f;
    }
}
