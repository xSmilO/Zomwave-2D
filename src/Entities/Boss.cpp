#include "Entities/Boss.h"
#include "Weapon.h"
#include "raylib.h"
#include "raymath.h"

Boss::Boss(Texture2D *texIdle, Texture2D *texWalk, Texture2D *texAttack,
           Vector2 startPos, BulletManager *bm, AudioManager *am) {
    audioManager = am;
    bulletManager = bm;
    position = startPos;
    width = 50.0f;
    height = 60.0f;

    std::vector<Vector2> idleFramePos = {{0, 0}, {1, 0}, {2, 0},  {3, 0},
                                         {4, 0}, {5, 0}, {6, 0},  {7, 0},
                                         {8, 0}, {9, 0}, {10, 0}, {11, 0}};

    std::vector<Vector2> walkFramePos = {{0, 0}, {1, 0}, {2, 0}, {3, 0},
                                         {4, 0}, {5, 0}, {6, 0}, {7, 0}};

    animator.AddAnimation("stop", texIdle, {128, 128}, 0, {{0, 0}}, true);

    animator.AddAnimation("idle", texIdle, {128, 128}, 10, idleFramePos, false);

    animator.AddAnimation("walk", texWalk, {128, 128}, 20, walkFramePos, true);

    animator.AddAnimation("attack", texAttack, {128, 128}, 10, walkFramePos,
                          false);

    animator.SetState("idle");
}

void Boss::Update(float dt, Vector2 playerPos, Map *map) {
    if (!active)
        return;

    if (attackTimer > 0.0f)
        attackTimer -= dt;
    animator.Update(dt);

    Vector2 dir = Vector2Subtract(playerPos, position);
    float distance = Vector2Length(dir);

    if (distance != 0.0f) {
        dir = Vector2Normalize(dir);
    }

    if (animator.IsAnimationFinished() && animator.GetState() == "attack") {
        Vector2 center = {position.x, position.y};

        float dx = playerPos.x - center.x;
        float dy = playerPos.y - center.y;
        float angleRad = atan2f(dy, dx);

        Vector2 arrowStart = {position.x + cosf(angleRad),
                              position.y + sinf(angleRad)};

        Vector2 baseDirection = {cosf(angleRad), sinf(angleRad)};
        Vector2 arrowTarget =
            Vector2Add(arrowStart, Vector2Scale(baseDirection, 1000.0f));

        bulletManager->Shoot(arrowStart, arrowTarget, damage,
                             BulletType::BOTTLE, 350.0f, 0, true);

        attackTimer = attackCooldown;
        audioManager->PlayBossDrink(position, playerPos);
        animator.SetState("idle");
        animator.ResetAnimation();
    }

    bool canMove =
        (animator.GetState() == "idle" && animator.IsAnimationFinished()) ||
        animator.GetState() == "walk";

    if (distance > stopRange && canMove) {
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

        texFlip = moveX < 0;
        if (moveY != 0.0f) {
            Rectangle futureHitboxY = {hitX, hitY + moveY, width, height};
            if (map->CheckHitbox(futureHitboxY) == false) {
                position.y += moveY;
            }
        }
    }

    if (distance < stopRange && attackTimer > 0.0f &&
        animator.GetState() != "idle") {
        animator.SetState("stop");
        // animator.ResetAnimation();
    }

    if (distance <= attackRange && attackTimer <= 0.0f &&
        animator.GetState() != "attack") {
        animator.SetState("attack");
        audioManager->PlayShoot(WeaponType::BOW);
        animator.ResetAnimation();
    }
}

void Boss::Draw() {
    // Rectangle destRec = GetHitbox();
    // DrawRectangleRec(destRec, RED);
    animator.Draw(
        {position.x, position.y - (height / 2), width * 2, height * 2},
        texFlip);

    DrawHealthBar();
}
Rectangle Boss::GetHitbox() {
    return {position.x - (width / 2.0f), position.y - (height / 2.0f), width,
            height};
}

void Boss::DrawHealthBar() {
    float currentHealth = (health < 0) ? 0 : health;
    float hpPercent = (float)currentHealth / maxHealth;

    float barWidth = width;
    float barHeight = 8.0f;

    Vector2 barPos = {position.x - (barWidth / 2.0f), position.y - 40.0f};
    DrawRectangle(barPos.x, barPos.y, barWidth, barHeight, RED);

    DrawRectangle(barPos.x, barPos.y, barWidth * hpPercent, barHeight, GREEN);
    DrawRectangleLines(barPos.x, barPos.y, barWidth, barHeight, BLACK);
}
