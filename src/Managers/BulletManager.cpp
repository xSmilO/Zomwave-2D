#include "Managers/BulletManager.h"
#include "Map.h"

void BulletManager::SpawnBullet(Vector2 startPos, Vector2 targetPos,
                                float speed) {
    bullets.push_back(Bullet(startPos, targetPos, speed));
}

void BulletManager::Update(Map *map) {
    for (auto &bullet : bullets) {
        bullet.Update(map);
    }

    std::erase_if(bullets, [](const Bullet &b) { return !b.active; });
}

void BulletManager::Draw() {
    for (auto &bullet : bullets) {
        bullet.Draw();
    }
}
