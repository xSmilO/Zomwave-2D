#include "Managers/EnemyManager.h"
#include "Entities/Zombie.h"
#include <memory>

EnemyManager::EnemyManager(ResourceManager *resources) {
    this->resources = resources;
}

void EnemyManager::SpawnZombie(Vector2 pos) {
    enemies.push_back(std::make_unique<Zombie>(&resources->zombieTex, pos));
}

void EnemyManager::Update(Vector2 playerPos, Map *map,
                          BulletManager *bulletManager) {
    for (auto &enemy : enemies) {
        enemy->Update(playerPos, map);

        for (auto &bullet : bulletManager->bullets) {
            if (bullet.active) {
                if (CheckCollisionCircleRec(bullet.position, bullet.radius,
                                            enemy->GetHitbox())) {
                    enemy->health--;
                    bullet.active = false;

                    if (enemy->health <= 0) {
                        enemy->active = false;
                    }
                }
            }
        }
    }

    std::erase_if(enemies,
                  [](const std::unique_ptr<Enemy> &e) { return !e->active; });
}

void EnemyManager::Draw() {
    for (auto &enemy : enemies) {
        enemy->Draw();
    }
}
