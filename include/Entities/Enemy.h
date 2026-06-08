#pragma once
#include "Managers/AudioManager.h"
#include "Map.h"
#include "raylib.h"

/**
 * @class Enemy
 * @brief Abstract base class for all enemy types.
 *
 * Provides common fields (health, speed, damage, position) and declares
 * pure-virtual update, draw, and hitbox methods that derived classes must implement.
 */
class Enemy {
  protected:
    float width, height;        ///< Sprite dimensions in pixels.
    AudioManager *audioManager; ///< Pointer to the audio manager for playing enemy sounds.

  public:
    Vector2 position;            ///< World position of the enemy.
    bool active = true;          ///< Whether the enemy is still alive and active.
    float health;                ///< Current health points.
    float maxHealth;             ///< Maximum health points.
    float attackCooldown = 1.0f; ///< Time (seconds) between attacks.
    float attackTimer = 0.0f;    ///< Accumulated time since last attack.
    float speed;                 ///< Movement speed in world units per second.
    float damage = 15;           ///< Damage dealt per attack.
    int killReward;              ///< Coins awarded for killing this enemy.
    int dropReward;              ///< Coins dropped on the ground upon death.

    virtual ~Enemy() = default;

    /**
     * @brief Updates the enemy's logic (movement, attack timers, AI).
     * @param dt       Delta time in seconds.
     * @param playerPos Current position of the player.
     * @param map       Pointer to the map for collision checks.
     */
    virtual void Update(float dt, Vector2 playerPos, Map *map) = 0;

    /// Draws the enemy sprite.
    virtual void Draw() = 0;

    /// @return The enemy's collision rectangle in world coordinates.
    virtual Rectangle GetHitbox() = 0;

    /**
     * @brief Draws a health bar above the enemy.
     */
    virtual void DrawHealthBar();

    /**
     * @brief Applies damage to the enemy.
     * @param damage Amount of damage to subtract from health.
     */
    virtual void TakeDamage(float damage);
};
