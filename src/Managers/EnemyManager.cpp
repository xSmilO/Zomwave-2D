#include "Managers/EnemyManager.h"
#include "Entities/Skeleton.h"
#include "Entities/Zombie.h"
#include "Managers/BulletManager.h"
#include "raymath.h"
#include <memory>

EnemyManager::EnemyManager(ResourceManager *resources, BulletManager *bm) {
    this->resources = resources;
    bulletManager = bm;
}

void EnemyManager::SpawnZombie(Vector2 pos) {
    enemies.push_back(std::make_unique<Zombie>(&resources->texZombie, pos));
}

void EnemyManager::SpawnSkeleton(Vector2 pos) {
    enemies.push_back(std::make_unique<Skeleton>(
        &resources->texSkeleton, &resources->texBow, pos, bulletManager));
}

void EnemyManager::Update(float dt, Player *player, Map *map,
                          BulletManager *bulletManager,
                          PickupManager *pickupManager) {
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
            if (bullet.active && !bullet.isEnemy) {
                if (CheckCollisionCircleRec(bullet.position, bullet.radius,
                                            enemy->GetHitbox())) {
                    enemy->health -= bullet.damage;
                    bullet.active = false;

                    if (enemy->health <= 0) {
                        enemy->active = false;
                        // dropnij coina
                        pickupManager->Spawn(enemy->position, PickupType::COIN);
                    }
                }
            }
        }
    }

    std::erase_if(enemies,
                  [](const std::unique_ptr<Enemy> &e) { return !e->active; });

    std::vector<Vector2> oldPositions(enemies.size());

    for (size_t i = 0; i < enemies.size(); i++) {
        oldPositions[i] = enemies[i]->position;
    }

    for (size_t i = 0; i < enemies.size(); i++) {
        for (size_t j = i + 1; j < enemies.size(); j++) {
            Enemy *e1 = enemies[i].get();
            Enemy *e2 = enemies[j].get();

            Vector2 diff = Vector2Subtract(e1->position, e2->position);
            float distance = Vector2Length(diff);

            float minDistance = 30.0f;

            if (distance < minDistance && distance > 0.01f) {
                float overlap = minDistance - distance;

                Vector2 direction = Vector2Normalize(diff);

                Vector2 pushVector = Vector2Scale(direction, overlap * 0.5f);

                e1->position = Vector2Add(e1->position, pushVector);
                e2->position = Vector2Subtract(e2->position, pushVector);
            }
        }
    }

    for (size_t i = 0; i < enemies.size(); i++) {
        if (map->CheckHitbox(enemies[i]->GetHitbox()))
            enemies[i]->position = oldPositions[i];
    }
}

void EnemyManager::Draw() {
    for (auto &enemy : enemies) {
        enemy->Draw();
    }
}

std::unique_ptr<Enemy> EnemyManager::CreateZombie() {
    return std::make_unique<Zombie>(&resources->texZombie, Vector2{0, 0});
}

std::unique_ptr<Enemy> EnemyManager::CreateSkeleton() {
    printf("szkielet tej \n");
    return std::make_unique<Skeleton>(&resources->texSkeleton,
                                      &resources->texBow, Vector2{0, 0},
                                      bulletManager);
}

void EnemyManager::AddEnemy(std::unique_ptr<Enemy> enemy) {
    enemies.push_back(std::move(enemy));
}
