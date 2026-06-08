#pragma once

#include "Managers/EnemyManager.h"
#include "Managers/GameBalance.h"
#include "Map.h"
#include "raylib.h"

/**
 * @class WaveManager
 * @brief Manages wave progression and enemy spawning over time.
 *
 * Each wave lasts a fixed duration. Enemies are spawned at intervals
 * that decrease as waves progress. Boss enemies spawn at specific
 * wave milestones.
 */
class WaveManager {
  private:
    int currentWave;            ///< Current wave number.
    float waveTimer = 0.0f;     ///< Elapsed time within the current wave.
    float waveDuration = 60.0f; ///< Duration of each wave in seconds.
    float globalTime = 0.0f;    ///< Total elapsed game time.

    float spawnTimer = 0.0f;  ///< Accumulated time since last enemy spawn.
    bool bossSpawned = false; ///< Whether the boss for the current wave has been spawned.

    /**
     * @brief Attempts to find a valid spawn position near the player.
     * @param playerPos Current player position.
     * @param enemy     Pointer to the enemy (for size reference).
     * @param map       Pointer to the map (for collision checks).
     * @return True if a valid position was found.
     */
    bool TryFindSpawnPos(Vector2 playerPos, Enemy *enemy, Map *map);

  public:
    /**
     * @brief Initialises the wave manager from game balance data.
     * @param gb Reference to the game balance config.
     */
    void Init(const GameBalance &gb);

    /**
     * @brief Updates wave logic: spawning enemies, advancing waves.
     * @param dt            Delta time in seconds.
     * @param enemyManager  Pointer to the enemy manager (for spawning).
     * @param playerPos     Current player position.
     * @param map           Pointer to the map (for spawn positions).
     * @param gb            Reference to the game balance config.
     */
    void Update(float dt, EnemyManager *enemyManager, Vector2 playerPos, Map *map, const GameBalance &gb);

    /// @return The current wave number.
    int GetCurrentWave() const {
        return currentWave;
    }

    /// @return Total elapsed game time in seconds.
    float GetGlobalTime() const {
        return globalTime;
    }
};
