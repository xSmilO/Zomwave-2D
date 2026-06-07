#include "Entities/Zombie.h"
#include "Map.h"
#include <raymath.h>
#include <vector>

Zombie::Zombie(Texture2D *texZombie, Vector2 startPos, AudioManager* am) {
    audioManager = am;
    position = startPos;
    speed = 100.0f;
    width = 24.0f;
    height = 24.0f;
    health = 60.0f;
    maxHealth = health;
    active = true;
    attackTimer = 0.0f;
    ambientTimer = GetRandomValue(300, 800) / 100.0f;

    std::vector<Vector2> walkFramePos = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};

    animator.AddAnimation("walk", texZombie, {32, 32}, 15, walkFramePos, true);

    animator.SetState("walk");
}

void Zombie::Update(float dt, Vector2 playerPos, Map *map) {
    if (!active)
        return;

    if (attackTimer > 0.0f) {
        attackTimer -= dt;
    }

    ambientTimer -= dt;
    if (ambientTimer <= 0.0f) {
        audioManager->PlayZombieAmbient(position,  playerPos);
        ambientTimer = GetRandomValue(300, 800) / 100.0f;
    }

    animator.Update(dt);

    Vector2 dir = Vector2Subtract(playerPos, position);
    dir = Vector2Normalize(dir);

    float moveX = dir.x * speed * dt;
    float moveY = dir.y * speed * dt;

    float hitX = position.x - (width / 2.0f);
    float hitY = position.y - (height / 2.0f);

    if (moveX != 0.0f) {
        Rectangle futureHitboxX = {hitX + moveX, hitY, width, height};

        if (map->CheckHitbox(futureHitboxX) == false) {
            position.x += moveX;
        }
    }

    if (moveY != 0.0f) {
        Rectangle futureHitboxY = {hitX, hitY + moveY, width, height};

        if (map->CheckHitbox(futureHitboxY) == false) {
            position.y += moveY;
        }
    }

    texFlip = moveX < 0;
}

void Zombie::Draw() {
    if (!active)
        return;

    Rectangle destRec = GetHitbox();
    DrawRectangleRec(destRec, RED);
    animator.Draw(
        {position.x, position.y - (height / 2), width * 2, height * 2},
        texFlip);

    DrawHealthBar();
}

Rectangle Zombie::GetHitbox() {
    return {position.x - (width / 2.0f), position.y - (height / 2.0f), width,
            height};
}
