#include "Managers/EnemyManager.h"
#include "Entities/Boss.h"
#include "Entities/Skeleton.h"
#include "Entities/Zombie.h"
#include "Managers/AudioManager.h"
#include "Managers/BulletManager.h"
#include "raymath.h"
#include <memory>

EnemyManager::EnemyManager(ResourceManager *rm, BulletManager *bm,
                           AudioManager *am) {
    resources = rm;
    bulletManager = bm;
    audioManager = am;
}

void EnemyManager::Update(float dt, Player *player, Map *map,
                          BulletManager *bulletManager,
                          CoinManager *coinManager) {
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
                    if (enemy->active == false)
                        break;

                    enemy->health -= bullet.damage;
                    bullet.active = false;

                    if (enemy->health <= 0) {
                        enemy->active = false;
                        player->coins += enemy->killReward;
                        coinManager->SpawnCoin(enemy->position,
                                               enemy->dropReward);
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

            float minDistance =
                std::max(e1->GetHitbox().height, e2->GetHitbox().height);

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

std::unique_ptr<Enemy> EnemyManager::CreateZombie(const GameBalance &gb) {
    if (gb.enemies.find("Zombie") == gb.enemies.end()) {
        printf("Error: can't find zombie in config.json!\n");
        return std::make_unique<Zombie>(&resources->texZombie, Vector2{0, 0},
                                        audioManager);
    }

    auto enemy = std::make_unique<Zombie>(&resources->texZombie, Vector2{0, 0},
                                          audioManager);

    const EnemyStats &stats = gb.enemies.at("Zombie");
    enemy->maxHealth = stats.health;
    enemy->health = enemy->maxHealth;
    enemy->killReward = stats.killReward;
    enemy->dropReward = stats.dropReward;
    enemy->attackCooldown = stats.attackCooldown;
    enemy->speed = stats.speed;
    enemy->damage = stats.damage;

    return enemy;
}

std::unique_ptr<Enemy> EnemyManager::CreateSkeleton(const GameBalance &gb) {
    if (gb.enemies.find("Skeleton") == gb.enemies.end()) {
        printf("Error: can't find skeleton in config.json!\n");
        return std::make_unique<Skeleton>(&resources->texSkeleton,
                                          &resources->texBow, Vector2{0, 0},
                                          bulletManager, audioManager);
    }
    auto enemy =
        std::make_unique<Skeleton>(&resources->texSkeleton, &resources->texBow,
                                   Vector2{0, 0}, bulletManager, audioManager);
    const EnemyStats &stats = gb.enemies.at("Skeleton");
    enemy->maxHealth = stats.health;
    enemy->health = enemy->maxHealth;
    enemy->killReward = stats.killReward;
    enemy->dropReward = stats.dropReward;
    enemy->attackCooldown = stats.attackCooldown;
    enemy->speed = stats.speed;
    enemy->damage = stats.damage;
    return enemy;
}

std::unique_ptr<Enemy> EnemyManager::CreateBoss(const GameBalance &gb) {
    if (gb.enemies.find("Boss") == gb.enemies.end()) {
        printf("Error: can't find boss in config.json!\n");
        return std::make_unique<Boss>(&resources->texBossIdle,
                                      &resources->texBossWalk,
                                      &resources->texBossAttack, Vector2{0, 0},
                                      bulletManager, audioManager);
    }

    auto enemy = std::make_unique<Boss>(
        &resources->texBossIdle, &resources->texBossWalk,
        &resources->texBossAttack, Vector2{0, 0}, bulletManager, audioManager);

    const EnemyStats &stats = gb.enemies.at("Boss");
    enemy->maxHealth = stats.health;
    enemy->health = enemy->maxHealth;
    enemy->killReward = stats.killReward;
    enemy->dropReward = stats.dropReward;
    enemy->attackCooldown = stats.attackCooldown;
    enemy->speed = stats.speed;
    enemy->damage = stats.damage;

    return enemy;
}

void EnemyManager::AddEnemy(std::unique_ptr<Enemy> enemy) {
    enemies.push_back(std::move(enemy));
}

void EnemyManager::Clear() {
    for (size_t i = 0; i < enemies.size(); i++) {
        enemies[i].reset();
    }

    enemies.clear();
}
