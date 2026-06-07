#pragma once
#include "nlohmann/detail/macro_scope.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

struct PlayerStats {
    int maxHealth;
    float speed;
    int coins;
    int potions;
    float invincibilityDuration;
    int potionBaseCost = 50;
    int potionFlatIncrease = 30;
    float potionMultiplier = 1.1f;
    std::vector<int> healthUpgradeCost;
    std::vector<float> maxHealthLevels;

    std::vector<int> speedUpgradeCost;
    std::vector<float> speedLevels;
    
    std::vector<int> visionUpgradeCost;
    std::vector<float> visionLevels;
};

struct WeaponStats {
    int unlockCost;
    std::vector<int> upgradeCost;
    std::vector<float> damage;
    std::vector<int> maxAmmo;
    std::vector<float> spread;
    std::vector<float> fireCooldown;
    std::vector<float> pellets;
};

struct EnemyStats {
    float health = 100.0f;
    float speed = 40.0f;
    float damage = 10.0f;
    float attackCooldown = 1.0f;

    int killReward = 0;
    int dropReward = 0;
};

struct WaveStats {
    int currentWave;
    float baseSpawnInterval = 2.0f;
    float waveMultiplier = 0.1f;
    float minSpawnInterval = 0.3f;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WaveStats, currentWave, baseSpawnInterval, waveMultiplier,
                                   minSpawnInterval)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerStats, coins, potionBaseCost,
                                   potionFlatIncrease, potionMultiplier,
                                   healthUpgradeCost, maxHealthLevels,
                                   speedUpgradeCost, speedLevels, visionUpgradeCost, visionLevels)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WeaponStats, unlockCost, upgradeCost, damage,
                                   maxAmmo, spread, fireCooldown, pellets)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EnemyStats, health, speed, damage,
                                   attackCooldown, killReward, dropReward)

struct GameBalance {
    PlayerStats player;
    std::unordered_map<std::string, WeaponStats> weapons;
    std::unordered_map<std::string, EnemyStats> enemies;
    WaveStats waves;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameBalance, player, weapons, enemies, waves)
