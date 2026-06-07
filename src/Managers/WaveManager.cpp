#include "Managers/WaveManager.h"
#include "raylib.h"
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

    if (candidates.empty()) {
        return false;
    }
    int maxAttempts = 10;
    for (int i = 0; i < maxAttempts; i++) {
        int randomIndex = GetRandomValue(0, candidates.size() - 1);
        enemy->position = candidates[randomIndex];

        if (map->CheckHitbox(enemy->GetHitbox()) == false) {
            return true;
        }
    }
    return false;
}

void WaveManager::Update(float dt, EnemyManager *enemyManager,
                         Vector2 playerPos, Map *map, const GameBalance &gb) {
    globalTime += dt;
    waveTimer += dt;

    if (waveTimer >= waveDuration) {
        currentWave++;
        waveTimer = 0.0f;
    }

    float currentSpawnInterval =
        gb.waves.baseSpawnInterval /
        (1.0f + (currentWave * gb.waves.waveMultiplier));

    if (currentSpawnInterval < gb.waves.minSpawnInterval) {
        currentSpawnInterval = gb.waves.minSpawnInterval;
    }

    spawnTimer += dt;
    if (spawnTimer >= currentSpawnInterval) {
        std::unique_ptr<Enemy> newEnemy = nullptr;

        int skeletonChance = 10 + (currentWave * 5);
        if (skeletonChance > 40)
            skeletonChance = 40;

        int bossChance = 0;
        if (currentWave >= 5) {
            bossChance = 2 + ((currentWave - 5) * 2);

            if (bossChance > 15)
                bossChance = 15;
        }

        int roll = GetRandomValue(1, 100);

        if (roll <= bossChance) {
            newEnemy = enemyManager->CreateBoss(gb);
        } else if (roll <= (bossChance + skeletonChance)) {
            newEnemy = enemyManager->CreateSkeleton(gb);
        } else {
            newEnemy = enemyManager->CreateZombie(gb);
        }

        if (newEnemy != nullptr) {
            if (TryFindSpawnPos(playerPos, newEnemy.get(), map)) {
                enemyManager->AddEnemy(std::move(newEnemy));
                spawnTimer = 0.0f;
            }
        }
    }
}

void WaveManager::Init(const GameBalance& gb) {
    currentWave = gb.waves.currentWave;
    waveTimer = 0.0f;
    waveDuration = 60.0f;
    globalTime = 0.0f;
}
