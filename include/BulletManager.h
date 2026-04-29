#pragma once
#include <vector>
#include "Bullet.h"
#include "Map.h"

class BulletManager {
  public:
    std::vector<Bullet> bullets;

    void SpawnBullet(Vector2 startPos, Vector2 targetPos, float speed);

    void Update(Map* map);
    void Draw();
};
