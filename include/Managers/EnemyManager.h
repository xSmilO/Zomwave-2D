#pragma once
#include "BulletManager.h"
#include "Entities/Enemy.h"
#include "Entities/Player.h"
#include "Managers/CoinManager.h"
#include "Managers/GameBalance.h"
#include "Managers/ResourceManager.h"
#include "Map.h"
#include <memory>
#include <vector>

/**
 * @class EnemyManager
 * @brief Manages all enemy entities: spawning, updating, and rendering.
 *
 * Owns a vector of unique_ptr<Enemy> and provides factory methods for
 * creating each enemy type (Zombie, Skeleton, Boss).
 */
class EnemyManager {
  private:
    ResourceManager *resources;   ///< Pointer to the resource manager for textures.
    BulletManager *bulletManager; ///< Pointer to the bullet manager (for enemy projectiles).
    AudioManager *audioManager;   ///< Pointer to the audio manager (for enemy sounds).

  public:
    /**
     * @brief Constructs the EnemyManager.
     * @param rm Pointer to the resource manager.
     * @param bm Pointer to the bullet manager.
     * @param am Pointer to the audio manager.
     */
    EnemyManager(ResourceManager *rm, BulletManager *bm, AudioManager *am);

    std::vector<std::unique_ptr<Enemy>> enemies; ///< List of all active enemies.

    /**
     * @brief Spawns a zombie at the given position.
     * @param pos World position.
     * @param gm  Pointer to the game balance config.
     */
    void SpawnZombie(Vector2 pos, GameBalance *gm);

    /**
     * @brief Spawns a skeleton at the given position.
     * @param pos World position.
     * @param gm  Pointer to the game balance config.
     */
    void SpawnSkeleton(Vector2 pos, GameBalance *gm);

    /**
     * @brief Adds a pre-created enemy to the manager.
     * @param enemy Unique pointer to the enemy.
     */
    void AddEnemy(std::unique_ptr<Enemy> enemy);

    /**
     * @brief Updates all enemies (AI, movement, attacks).
     * @param dt             Delta time in seconds.
     * @param player         Pointer to the player.
     * @param map            Pointer to the map for collision checks.
     * @param bulletManager  Pointer to the bullet manager.
     * @param coinManager    Pointer to the coin manager (for drops).
     */
    void Update(float dt, Player *player, Map *map, BulletManager *bulletManager, CoinManager *coinManager);

    /// Draws all active enemies.
    void Draw();

    /// Removes all enemies from the scene.
    void Clear();

    /// @return A new Zombie enemy configured from the game balance.
    std::unique_ptr<Enemy> CreateZombie(const GameBalance &gb);

    /// @return A new Skeleton enemy configured from the game balance.
    std::unique_ptr<Enemy> CreateSkeleton(const GameBalance &gb);

    /// @return A new Boss enemy configured from the game balance.
    std::unique_ptr<Enemy> CreateBoss(const GameBalance &gb);
};
