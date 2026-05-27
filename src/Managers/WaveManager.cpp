#include "Managers/WaveManager.h"
#include <cmath>

bool WaveManager::TryFindSpawnPos(Vector2 playerPos, Enemy *enemy, Map *map) {
    for (int i = 0; i < 10; i++) {
        float randomAngle = GetRandomValue(0, 360) * DEG2RAD;
        float spawnRadius = GetRandomValue(300, 600);

        Vector2 testPos = {playerPos.x + (float)cos(randomAngle) * spawnRadius,
                           playerPos.y + (float)sin(randomAngle) * spawnRadius};
        enemy->position = testPos;

        if (map->CheckHitbox(enemy->GetHitbox()) == false) {
            return true;
        }
    }

    return false;
}

void WaveManager::Update(float dt, EnemyManager *enemyManager,
                         Vector2 playerPos, Map *map) {
    globalTime += dt;
    waveTimer += dt;
    if (waveTimer >= waveDuration) {
        currentWave++;
        waveTimer = 0.0f;
        bossSpawned = false;
    }

    float currentSpawnInterval = 1.5f / (1.0f) + (currentWave * 1.15f);

    if (currentSpawnInterval < 0.15f)
        currentSpawnInterval = 0.15f;

    spawnTimer += dt;
    if (spawnTimer >= currentSpawnInterval) {
        std::unique_ptr<Enemy> newEnemy = nullptr;
        int skeletonChance = 10 + (currentWave * 5);

        if (skeletonChance > 40) {
            skeletonChance = 40;
        }

        int roll = GetRandomValue(1, 100);

        if (roll <= skeletonChance) {
            newEnemy = enemyManager->CreateSkeleton();
        } else {
            newEnemy = enemyManager->CreateZombie();
        }

        if (newEnemy != nullptr) {
            if (TryFindSpawnPos(playerPos, newEnemy.get(), map)) {
                enemyManager->AddEnemy(std::move(newEnemy));
                spawnTimer = 0.0f;
            }
        }
    }
}
