#pragma once
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
};

struct WeaponStats {
    int unlockCost;
    std::vector<int> upgradeCost;
    std::vector<float> damage;
    std::vector<int> maxAmmo;
    std::vector<float> spread;
    std::vector<float> fireCooldown;
};

struct EnemyStats {
    float health = 100.0f;
    float speed = 40.0f;
    float damage = 10.0f;
    float attackCooldown = 1.0f;

    int killReward = 0;
    int dropReward = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerStats, coins, potionBaseCost,
                                   potionFlatIncrease, potionMultiplier,
                                   healthUpgradeCost, maxHealthLevels,
                                   speedUpgradeCost, speedLevels)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WeaponStats, unlockCost, upgradeCost, damage,
                                   maxAmmo, spread, fireCooldown)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EnemyStats, health, speed, damage, attackCooldown, killReward, dropReward)

struct GameBalance {
    PlayerStats player;
    std::unordered_map<std::string, WeaponStats> weapons;
    std::unordered_map<std::string, EnemyStats> enemies;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameBalance, player, weapons, enemies)
