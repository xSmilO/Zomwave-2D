#pragma once

#include "Map.h"
#include "raylib.h"
#include <vector>

enum class BulletType { BULLET, ARROW, BOTTLE };

struct Bullet {
    Vector2 position;
    Vector2 velocity;
    float damage;
    bool active;
    float lifeTimer;
    float radius;
    float scale;
    bool isEnemy;
    BulletType type;

    Rectangle GetHitbox() const {
        return {position.x - radius, position.y - radius, radius * 2.0f,
                radius * 2.0f};
    }
};

class BulletManager {
  public:
    std::vector<Bullet> bullets;

    Texture2D *texBullet;
    Texture2D *texArrow;
    Texture2D *texBottle;

    BulletManager(Texture2D *texBullet, Texture2D *texArrow, Texture2D *texBottle);
    void Shoot(Vector2 startPos, Vector2 targetPos, float damage,
               BulletType bulletType, float speed, float spreadAngle, bool isEnemy);
    void Update(float dt, Map *map);
    void Draw();
};
