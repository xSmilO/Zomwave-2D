#pragma once
#include "nlohmann/detail/macro_scope.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

/**
 * @struct PlayerStats
 * @brief Player-related balance values loaded from config.
 */
struct PlayerStats {
    int coins;                     ///< Starting coin count.
    int potions;                   ///< Starting potion count.
    int bombs;                     ///< Starting bomb count.
    float invincibilityDuration;   ///< Invincibility duration after taking damage (seconds).
    int potionBaseCost = 50;       ///< Base cost of a health potion.
    int potionFlatIncrease = 30;   ///< Flat cost increase per potion purchased.
    float potionMultiplier = 1.1f; ///< Multiplicative cost increase per potion purchased.

    std::vector<int> healthUpgradeCost; ///< Cost per max-health upgrade level.
    std::vector<float> maxHealthLevels; ///< Max-health values per upgrade level.

    std::vector<int> speedUpgradeCost; ///< Cost per speed upgrade level.
    std::vector<float> speedLevels;    ///< Speed values per upgrade level.

    std::vector<int> visionUpgradeCost; ///< Cost per vision-radius upgrade level.
    std::vector<float> visionLevels;    ///< Vision-radius values per upgrade level.
};

/**
 * @struct WeaponStats
 * @brief Weapon balance values per upgrade level, loaded from config.
 */
struct WeaponStats {
    int unlockCost;                  ///< Coin cost to unlock the weapon.
    std::vector<int> upgradeCost;    ///< Cost per upgrade level.
    std::vector<float> damage;       ///< Damage per upgrade level.
    std::vector<int> maxAmmo;        ///< Max ammo per upgrade level.
    std::vector<float> spread;       ///< Spread angle per upgrade level.
    std::vector<float> fireCooldown; ///< Fire cooldown (seconds) per upgrade level.
    std::vector<float> pellets;      ///< Pellets per shot per upgrade level.
};

/**
 * @struct EnemyStats
 * @brief Enemy balance values loaded from config.
 */
struct EnemyStats {
    float health = 100.0f;       ///< Enemy health points.
    float speed = 40.0f;         ///< Enemy movement speed.
    float damage = 10.0f;        ///< Enemy damage per attack.
    float attackCooldown = 1.0f; ///< Time between attacks (seconds).

    int killReward = 0; ///< Coins awarded on kill.
    int dropReward = 0; ///< Coins dropped on the ground on death.
};

/**
 * @struct WaveStats
 * @brief Wave-spawning balance values loaded from config.
 */
struct WaveStats {
    int currentWave;                ///< Starting wave number.
    float baseSpawnInterval = 2.0f; ///< Base time between spawns (seconds).
    float waveMultiplier = 0.1f;    ///< Spawn-interval reduction per wave.
    float minSpawnInterval = 0.3f;  ///< Minimum spawn interval cap.
};

/**
 * @struct BombStats
 * @brief Bomb balance values loaded from config.
 */
struct BombStats {
    int cost = 200;        ///< Coin cost to buy a bomb.
    float damage = 100.0f; ///< Explosion damage.
    float radius = 200.0f; ///< Explosion radius.
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WaveStats, currentWave, baseSpawnInterval, waveMultiplier, minSpawnInterval)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerStats, coins, potions, bombs, invincibilityDuration, potionBaseCost,
                                   potionFlatIncrease, potionMultiplier, healthUpgradeCost, maxHealthLevels,
                                   speedUpgradeCost, speedLevels, visionUpgradeCost, visionLevels)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WeaponStats, unlockCost, upgradeCost, damage, maxAmmo, spread, fireCooldown, pellets)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EnemyStats, health, speed, damage, attackCooldown, killReward, dropReward)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BombStats, cost, damage, radius)

/**
 * @struct GameBalance
 * @brief Top-level container for all game balance configuration.
 *
 * Loaded from a JSON file via ConfigManager. Contains player stats,
 * weapon stats per weapon name, enemy stats per enemy name, wave
 * parameters, and bomb parameters.
 */
struct GameBalance {
    PlayerStats player;                                   ///< Player-related balance values.
    std::unordered_map<std::string, WeaponStats> weapons; ///< Weapon stats keyed by weapon name.
    std::unordered_map<std::string, EnemyStats> enemies;  ///< Enemy stats keyed by enemy name.
    WaveStats waves;                                      ///< Wave-spawning parameters.
    BombStats bomb;                                       ///< Bomb parameters.
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameBalance, player, weapons, enemies, waves, bomb)
