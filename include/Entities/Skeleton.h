#pragma once

#include "Animator.h"
#include "Entities/Enemy.h"
#include "Managers/AudioManager.h"
#include "Managers/BulletManager.h"
#include "Map.h"
#include "raylib.h"

/**
 * @class Skeleton
 * @brief A ranged enemy that shoots arrows at the player from a distance.
 *
 * Inherits from Enemy. Uses a bow animation and fires projectiles via
 * the BulletManager. Keeps its distance and retreats when the player
 * gets too close.
 */
class Skeleton : public Enemy {
  private:
    float attackRange = 200.0f; ///< Maximum distance at which the skeleton will attack.
    float stopRange = 150.0f;   ///< Distance at which the skeleton stops moving toward the player.
    float bowRotation;          ///< Rotation angle of the bow towards the target.
    float bowWidth = 16.0f;     ///< Bow sprite width in pixels.
    float bowHeight = 16.0f;    ///< Bow sprite height in pixels.
    Animator animator;          ///< Animator for the skeleton body.
    bool texFlip;               ///< Whether the body sprite is flipped horizontally.

    Animator bowAnimator; ///< Animator for the bow sprite.
    Vector2 bowPosition;  ///< Calculated world position of the bow.

    BulletManager *bulletManager; ///< Pointer to the bullet manager for firing arrows.

  public:
    /**
     * @brief Constructs a Skeleton enemy.
     * @param texCharacter Pointer to the skeleton body spritesheet.
     * @param texBow       Pointer to the bow spritesheet.
     * @param startPos     Initial world position.
     * @param bm           Pointer to the bullet manager.
     * @param am           Pointer to the audio manager.
     */
    Skeleton(Texture2D *texCharacter, Texture2D *texBow, Vector2 startPos, BulletManager *bm, AudioManager *am);

    /**
     * @brief Calculates the bow position and rotation to aim at a target.
     * @param targetPos The target world position.
     */
    void CalculateBowPos(Vector2 targetPos);

    void Update(float dt, Vector2 playerPos, Map *map) override;
    void Draw() override;
    Rectangle GetHitbox() override;
};
