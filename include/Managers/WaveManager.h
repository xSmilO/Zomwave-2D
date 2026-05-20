#pragma once

#include "Managers/EnemyManager.h"
#include "Map.h"
#include "raylib.h"

class WaveManager {
  private:
    int currentWave = 1;
    float waveTimer = 0.0f;
    float waveDuration = 60.0f;
    float globalTime = 0.0f;

    float spawnTimer = 0.0f;
    bool bossSpawned = false;

    bool TryFindSpawnPos(Vector2 playerPos, Enemy *enemy, Map *map);

  public:
    void Update(float dt, EnemyManager *enemyManager, Vector2 playerPos,
                Map *map);

    int GetCurrentWave() const { return currentWave; }
    float GetGlobalTime() const { return globalTime; }
};
