#pragma once

#include "Map.h"
#include "raylib.h"
#include <vector>

/**
 * @enum BulletType
 * @brief Identifies the visual type of a projectile.
 */
enum class BulletType {
    BULLET,
    ARROW,
    BOTTLE
};

/**
 * @struct Bullet
 * @brief Represents a single projectile in flight.
 */
struct Bullet {
    Vector2 position; ///< Current world position.
    Vector2 velocity; ///< Velocity vector (speed + direction).
    float damage;     ///< Damage dealt on impact.
    bool active;      ///< Whether the bullet is still active.
    float lifeTimer;  ///< Elapsed lifetime (used for auto-removal).
    float radius;     ///< Collision / visual radius.
    float scale;      ///< Visual scale multiplier.
    bool isEnemy;     ///< True if fired by an enemy, false if by the player.
    BulletType type;  ///< Visual type of the projectile.

    /// @return The collision rectangle for this bullet.
    Rectangle GetHitbox() const {
        return {position.x - radius, position.y - radius, radius * 2.0f, radius * 2.0f};
    }
};

/**
 * @class BulletManager
 * @brief Manages all projectile bullets in the game.
 *
 * Handles shooting (player and enemy), updating positions, collision
 * with map walls, and rendering the appropriate bullet textures.
 */
class BulletManager {
  public:
    std::vector<Bullet> bullets; ///< List of all active bullets.

    Texture2D *texBullet; ///< Texture for standard bullets.
    Texture2D *texArrow;  ///< Texture for arrows.
    Texture2D *texBottle; ///< Texture for bottle projectiles.

    /**
     * @brief Constructs the BulletManager with projectile textures.
     * @param texBullet Pointer to the bullet texture.
     * @param texArrow  Pointer to the arrow texture.
     * @param texBottle Pointer to the bottle texture.
     */
    BulletManager(Texture2D *texBullet, Texture2D *texArrow, Texture2D *texBottle);

    /**
     * @brief Fires a projectile from startPos toward targetPos.
     * @param startPos    Origin position.
     * @param targetPos   Target position.
     * @param damage      Damage dealt on hit.
     * @param bulletType  Visual type of the projectile.
     * @param speed       Projectile speed in world units per second.
     * @param spreadAngle Random angular spread applied to the direction.
     * @param isEnemy     True if fired by an enemy.
     */
    void Shoot(Vector2 startPos, Vector2 targetPos, float damage, BulletType bulletType, float speed, float spreadAngle,
               bool isEnemy);

    /**
     * @brief Updates all bullets (movement, lifetime, wall collision).
     * @param dt  Delta time in seconds.
     * @param map Pointer to the map for wall-collision checks.
     */
    void Update(float dt, Map *map);

    /// Draws all active bullets.
    void Draw();

    /// Removes all bullets from the scene.
    void Clear();
};
