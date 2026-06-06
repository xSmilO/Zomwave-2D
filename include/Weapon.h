#pragma once

#include "Animator.h"
#include "Managers/GameBalance.h"
#include "raylib.h"
#include <string>
enum class WeaponType { GLOCK, SHOTGUN, AK47, MP5, BOW };

enum class WeaponState { IDLE, SHOOTING, RELOADING };

struct Weapon {
    std::string name;
    WeaponType type;

    int width = 10;
    int height = 10;
    bool isUnlocked = false;
    int unlockCost = 0;
    int currentLevel = 0;
    int maxLevel = 0;
    int upgradeCost = 0;
    float damage = 0.0f;
    float fireCooldown = 0.0f;
    int pellets = 1;
    float spread = 1;
    Vector2 barrelOffest = {0, 0};
    Animator animator;
    int currentAmmo = 10;
    int maxAmmo = 10;
    WeaponState state = WeaponState::IDLE;

    void LoadStatsFromBalance(const GameBalance &balance) {
        const WeaponStats &stats = balance.weapons.at(this->name);

        this->unlockCost = stats.unlockCost;
        this->maxLevel = stats.damage.size();

        int idx = this->currentLevel - 1;
        if (idx < 0)
            idx = 0;
        if (idx >= this->maxLevel)
            idx = this->maxLevel - 1;

        this->damage = stats.damage[idx];
        this->maxAmmo = stats.maxAmmo[idx];
        this->spread = stats.spread[idx];
        this->fireCooldown = stats.fireCooldown[idx];
        this->upgradeCost = stats.upgradeCost[idx];

        this->currentAmmo = this->maxAmmo;

        printf("fire cooldown for %s %f\n", this->name.c_str(), this->fireCooldown);
    }
};
