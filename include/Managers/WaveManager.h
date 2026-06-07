#pragma once

#include "Managers/EnemyManager.h"
#include "Managers/GameBalance.h"
#include "Map.h"
#include "raylib.h"

class WaveManager {
  private:
    int currentWave;
    float waveTimer = 0.0f;
    float waveDuration = 60.0f;
    float globalTime = 0.0f;

    float spawnTimer = 0.0f;
    bool bossSpawned = false;

    bool TryFindSpawnPos(Vector2 playerPos, Enemy *enemy, Map *map);

  public:
    void Init(const GameBalance& gb);
    void Update(float dt, EnemyManager *enemyManager, Vector2 playerPos,
                Map *map, const GameBalance& gb);

    int GetCurrentWave() const { return currentWave; }
    float GetGlobalTime() const { return globalTime; }
};
