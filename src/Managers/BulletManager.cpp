#include "Managers/BulletManager.h"
#include "raylib.h"
#include "raymath.h"

BulletManager::BulletManager(Texture2D *texBullet, Texture2D *texArrow, Texture2D *texBottle) {
    this->texBullet = texBullet;
    this->texArrow = texArrow;
    this->texBottle = texBottle;
}

void BulletManager::Shoot(Vector2 startPos, Vector2 targetPos, float damage,
                          BulletType bulletType, float speed, float spreadAngle,
                          bool isEnemy) {
    Vector2 direction = Vector2Subtract(targetPos, startPos);

    float angle = atan2(direction.y, direction.x);

    angle += spreadAngle * DEG2RAD;

    Vector2 finalDirection = {cos(angle), sin(angle)};

    finalDirection = Vector2Normalize(finalDirection);
    Vector2 velocity = Vector2Scale(finalDirection, speed);

    Bullet b;
    b.position = startPos;
    b.velocity = velocity;
    b.damage = damage;
    b.lifeTimer = 2.0f;
    b.radius = 2.0f;
    b.active = true;
    b.type = bulletType;
    b.isEnemy = isEnemy;

    if (bulletType == BulletType::BULLET) {
        b.radius = 2.0f;
    }

    if (bulletType == BulletType::ARROW) {
        b.radius = 6.0f;
    }

    if (bulletType == BulletType::BOTTLE) {
        b.radius = 8.0f;
    }

    bullets.push_back(b);
}

void BulletManager::Update(float dt, Map *map) {
    for (auto &b : bullets) {
        if (!b.active)
            continue;

        b.position = Vector2Add(b.position, Vector2Scale(b.velocity, dt));
        b.lifeTimer -= dt;
        if (b.lifeTimer <= 0.0f) {
            b.active = false;
        }

        if (map->CheckHitbox(b.GetHitbox()))
            b.active = false;
    }

    std::erase_if(bullets, [](const Bullet &b) { return !b.active; });
}

void BulletManager::Draw() {
    for (const auto &b : bullets) {
        Texture2D *texToDraw = nullptr;

        if (b.type == BulletType::BULLET) {
            texToDraw = texBullet;
        } else if (b.type == BulletType::ARROW) {
            texToDraw = texArrow;
        } else if (b.type == BulletType::BOTTLE) {
            texToDraw = texBottle;
        }

        float rotation = atan2(b.velocity.y, b.velocity.x) * RAD2DEG;

        if(b.type == BulletType::BOTTLE)
            rotation += 90;

        Rectangle source = {0, 0, (float)texToDraw->width,
                            (float)texToDraw->height};

        Rectangle dest = {b.position.x, b.position.y, (float)texToDraw->width,
                          (float)texToDraw->height};


        Vector2 origin = {(float)texToDraw->width / 2,
                          (float)texToDraw->height / 2};

        DrawRectangleRec(b.GetHitbox(), GREEN);
        DrawTexturePro(*texToDraw, source, dest, origin, rotation, WHITE);
    }
}
