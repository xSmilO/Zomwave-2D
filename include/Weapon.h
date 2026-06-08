#pragma once

#include "Animator.h"
#include "Managers/GameBalance.h"
#include "raylib.h"
#include <string>

/**
 * @enum WeaponType
 * @brief Identifies the different weapon types available in the game.
 */
enum class WeaponType {
    GLOCK,
    SHOTGUN,
    AK47,
    MP5,
    BOW
};

/**
 * @enum WeaponState
 * @brief Represents the current state of a weapon.
 */
enum class WeaponState {
    IDLE,
    SHOOTING,
    RELOADING
};

/**
 * @struct Weapon
 * @brief Holds all runtime data for a single weapon instance.
 *
 * Each weapon has its own stats (damage, ammo, fire rate, spread),
 * an upgrade level, and an animator for visual feedback (shoot/reload).
 */
struct Weapon {
    std::string name; ///< Display name of the weapon.
    WeaponType type;  ///< Weapon type identifier.

    int width = 10;                        ///< Sprite width in pixels.
    int height = 10;                       ///< Sprite height in pixels.
    bool isUnlocked = false;               ///< Whether the player has unlocked this weapon.
    int unlockCost = 0;                    ///< Coin cost to unlock the weapon.
    int currentLevel = 0;                  ///< Current upgrade level (0 = base).
    int maxLevel = 0;                      ///< Maximum upgrade level.
    int upgradeCost = 0;                   ///< Coin cost for the next upgrade.
    float damage = 0.0f;                   ///< Damage dealt per hit/pellet.
    float fireCooldown = 0.0f;             ///< Minimum time (seconds) between shots.
    int pellets = 1;                       ///< Number of pellets fired per shot (shotgun).
    float spread = 1;                      ///< Angular spread of projectiles.
    Vector2 barrelOffest = {0, 0};         ///< Offset from weapon origin to barrel tip.
    Animator animator;                     ///< Animator for shoot/reload/idle animations.
    int currentAmmo = 10;                  ///< Ammo remaining in the current magazine.
    int maxAmmo = 10;                      ///< Maximum ammo capacity per magazine.
    WeaponState state = WeaponState::IDLE; ///< Current weapon state.

    /**
     * @brief Loads weapon stats from a GameBalance object based on the current level.
     * @param balance Reference to the game balance configuration.
     */
    void LoadStatsFromBalance(const GameBalance &balance) {
        const WeaponStats &stats = balance.weapons.at(this->name);

        this->unlockCost = stats.unlockCost;
        this->maxLevel = stats.damage.size();

        int idx = this->currentLevel - 1;
        if(idx < 0)
            idx = 0;
        if(idx >= this->maxLevel)
            idx = this->maxLevel - 1;

        this->damage = stats.damage[idx];
        this->maxAmmo = stats.maxAmmo[idx];
        this->spread = stats.spread[idx];
        this->fireCooldown = stats.fireCooldown[idx];
        this->upgradeCost = stats.upgradeCost[idx];
        this->pellets = stats.pellets[idx];

        this->currentAmmo = this->maxAmmo;
    }
};
