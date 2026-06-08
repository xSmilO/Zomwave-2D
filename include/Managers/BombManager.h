#pragma once
#include "Entities/Player.h"
#include "Managers/AudioManager.h"
#include "Managers/EnemyManager.h"
#include <raylib.h>
#include <raymath.h>
#include <vector>

/**
 * @struct Bomb
 * @brief Represents a thrown bomb in flight or exploding.
 */
struct Bomb {
    Vector2 startPos;     ///< Initial throw position.
    Vector2 currentPos;   ///< Current position during flight.
    Vector2 targetPos;    ///< Target landing position.
    float flightTimer;    ///< Elapsed time since throw.
    float flightDuration; ///< Total flight duration before impact.

    bool isExploding = false;    ///< Whether the bomb is in its explosion phase.
    float explosionTimer = 0.0f; ///< Elapsed time since explosion started.
};

/**
 * @class BombManager
 * @brief Manages bomb throwing, flight, explosion, and damage application.
 */
class BombManager {
  public:
    std::vector<Bomb> bombs; ///< List of active bombs.

    float explosionRadius = 200.0f; ///< Radius of the explosion area.
    float damage = 250.0f;          ///< Damage dealt by the explosion.

    /**
     * @brief Throws a bomb from start to target position.
     * @param start  The thrower's position.
     * @param target The target landing position.
     */
    void ThrowBomb(Vector2 start, Vector2 target);

    /**
     * @brief Updates all active bombs (flight, explosion, damage).
     * @param dt            Delta time in seconds.
     * @param player        Pointer to the player (for knockback, etc.).
     * @param enemyManager  Pointer to the enemy manager (for damage application).
     * @param audioManager  Pointer to the audio manager (for explosion sounds).
     */
    void Update(float dt, Player *player, EnemyManager *enemyManager, AudioManager *audioManager);

    /**
     * @brief Draws all active bombs.
     * @param texBomb The bomb texture to render.
     */
    void Draw(Texture2D texBomb);
};
