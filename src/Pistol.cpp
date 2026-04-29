#include "Pistol.h"
#include "BulletManager.h"
#include "raylib.h"
#include "raymath.h"

Pistol::Pistol() {
    std::vector<Vector2> shootFramePos = {{0, 0}};

    animator.AddAnimation("shoot", "../assets/pistol.png", {64, 32}, 1,
                          shootFramePos, false);

    animator.SetState("shoot");
    fireRate = 0.1;
    fireTimer = fireRate;
}

void Pistol::Update(Vector2 position, float rotation, float dt) {
    _position = {position.x, position.y, 50, 30};
    _rotation = rotation;
    animator.Update();
    fireTimer += dt;
}

void Pistol::Draw() {
    animator.Draw(_position, false, _rotation);
    // DrawRectangleRec(_position, RED);
}

void Pistol::Shoot(Vector2 targetPos, BulletManager *bulletManager) {
    // if (fire)
    if (fireTimer >= fireRate) {
        Vector2 direction =
            Vector2Subtract(targetPos, {_position.x, _position.y});
        direction = Vector2Normalize(direction);

        bulletManager->SpawnBullet({_position.x, _position.y}, targetPos, 800);

        fireTimer = 0.0f;
    }
}
