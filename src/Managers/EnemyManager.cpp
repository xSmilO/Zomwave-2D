#include "Managers/EnemyManager.h"
#include "Entities/Zombie.h"
#include <memory>

EnemyManager::EnemyManager(ResourceManager *resources) {
    this->resources = resources;
}

void EnemyManager::SpawnZombie(Vector2 pos) {
    enemies.push_back(std::make_unique<Zombie>(&resources->zombieTex, pos));
}

void EnemyManager::Update(float dt, Player *player, Map *map,
                          BulletManager *bulletManager) {
    for (auto &enemy : enemies) {
        enemy->Update(dt, player->GetPosition(), map);

        if (CheckCollisionRecs(enemy->GetHitbox(), player->GetHitbox())) {
            if (enemy->attackTimer <= 0.0f &&
                player->invincibilityTimer <= 0.0f) {
                player->TakeDamage(enemy->damage);

                enemy->attackTimer = enemy->attackCooldown;
            }
        }

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

std::unique_ptr<Enemy> EnemyManager::CreateZombie() {
    return std::make_unique<Zombie>(&resources->zombieTex, Vector2{0, 0});
}

void EnemyManager::AddEnemy(std::unique_ptr<Enemy> enemy) {
    enemies.push_back(std::move(enemy));
}
