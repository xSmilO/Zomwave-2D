#include "Bullet.h"
#include "raymath.h"

Bullet::Bullet(Vector2 startPos, Vector2 targetPos, float bulletSpeed) {
    position = startPos;
    speed = bulletSpeed;
    radius = 4.0f;
    active = true;

    Vector2 dir = Vector2Subtract(targetPos, startPos);

    direction = Vector2Normalize(dir);
}

void Bullet::Update(Map *map) {
    if (!active)
        return;

    float dt = GetFrameTime();

    position.x += direction.x * speed * dt;
    position.y += direction.y * speed * dt;

    Rectangle hitbox = {position.x - radius, position.y - radius, radius * 2.0f,
                        radius * 2.0f};

    if (map->CheckHitbox(hitbox)) {
        active = false;
    }
}

void Bullet::Draw() {
    if (!active)
        return;
    DrawCircleV(position, radius, YELLOW);
}
