#include "Managers/WaveManager.h"
#include "raymath.h"

bool WaveManager::TryFindSpawnPos(Vector2 playerPos, Enemy *enemy, Map *map) {
    if (map->validSpawnPoints.empty())
        return false;

    float minRadius = 300.0f;
    float maxRadius = 600.0f;

    std::vector<Vector2> candidates;
    for (const auto &spawnPos : map->validSpawnPoints) {
        float distance = Vector2Distance(playerPos, spawnPos);
        if (distance >= minRadius && distance <= maxRadius) {
            candidates.push_back(spawnPos);
        }
    }


    if (!candidates.empty()) {
        int randomIndex = GetRandomValue(0, candidates.size() - 1);
        enemy->position = candidates[randomIndex];

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

    currentSpawnInterval = 0.5f;

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
