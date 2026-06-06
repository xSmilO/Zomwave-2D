#include "Entities/Boss.h"
#include "Weapon.h"
#include "raylib.h"
#include "raymath.h"
#include <cmath>

Boss::Boss(Texture2D *texCharacter, Texture2D *texBow, Vector2 startPos,
                   BulletManager *bm, AudioManager *am) {
    audioManager = am;
    bulletManager = bm;
    position = startPos;
    width = 40.0f;
    height = 30.0f;

    std::vector<Vector2> walkFramePos = {{0, 0}, {1, 0}, {2, 0}, {3, 0},
                                         {4, 0}, {5, 0}, {6, 0}, {7, 0}};

    animator.AddAnimation("idle", texCharacter, {20, 20}, 0, {{0, 0}}, true);

    animator.AddAnimation("walk", texCharacter, {20, 20}, 10, walkFramePos,
                          true);
    animator.AddAnimation("attack", texCharacter, {20, 20}, 0, {{0,0}}, false);
    animator.SetState("walk");

    std::vector<Vector2> bowFramePos = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
}


void Boss::Update(float dt, Vector2 playerPos, Map *map) {
    if (!active)
        return;

    if (fireTimer > 0.0f)
        fireTimer -= dt;

    animator.Update(dt);

    Vector2 dir = Vector2Subtract(playerPos, position);
    float distance = Vector2Length(dir);

    if (distance != 0.0f) {
        dir = Vector2Normalize(dir);
    }

    // if (bowAnimator.IsAnimationFinished() && animator.GetState() == "attack") {
    //     float angleRad = bowRotation * DEG2RAD;
    //
    //     Vector2 arrowStart = {position.x + cosf(angleRad),
    //                           position.y + sinf(angleRad)};
    //
    //     Vector2 baseDirection = {cosf(angleRad), sinf(angleRad)};
    //     Vector2 arrowTarget =
    //         Vector2Add(arrowStart, Vector2Scale(baseDirection, 1000.0f));
    //
    //     bulletManager->Shoot(arrowStart, arrowTarget, 20, BulletType::ARROW,
    //                          500.0f, 0, true);
    //
    //     bowAnimator.SetState("idle");
    //     fireTimer = fireRate;
    // }

    if (distance > stopRange) {
        animator.SetState("walk");
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
    } else {
        animator.SetState("idle");
    }

    if (distance <= attackRange && fireTimer <= 0.0f) {
        audioManager->PlayShoot(WeaponType::BOW);
    }
}

void Boss::Draw() {
    Rectangle destRec = GetHitbox();
    DrawRectangleRec(destRec, RED);
    animator.Draw(
        {position.x, position.y - (height / 2), width * 2, height * 2}, false);

    DrawHealthBar();
}
Rectangle Boss::GetHitbox() {
    return {position.x - (width / 2.0f), position.y - (height / 2.0f), width,
            height};
}
