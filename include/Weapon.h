#pragma once

#include "Animator.h"
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

    int currentAmmo = 10;
    int maxAmmo = 10;
    float reloadTime = 1.5f;
    float reloadTimer = 0.0f;

    WeaponState state = WeaponState::IDLE;

    Animator animator;

    // Weapon(const Weapon &) = delete;
    // Weapon &operator=(const Weapon &) = delete;
};
