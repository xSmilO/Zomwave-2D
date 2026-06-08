#pragma once

#include "Animator.h"
#include "Entities/Enemy.h"
#include "Managers/AudioManager.h"
#include "Managers/BulletManager.h"
#include "Map.h"
#include "raylib.h"

/**
 * @class Boss
 * @brief A powerful boss enemy with ranged attacks and multiple animations.
 *
 * Inherits from Enemy. Has idle, walk, and attack animations. Fires
 * projectiles at the player via the BulletManager and periodically
 * plays ambient/drink sounds.
 */
class Boss : public Enemy {
  private:
    float attackRange =
        200.0f; ///< Maximum distance at which the boss will attack.
    float stopRange =
        150.0f; ///< Distance at which the boss stops moving toward the player.
    float ambientTimer; ///< Timer for ambient sound playback.
    Animator animator;  ///< Animator for boss animations (idle/walk/attack).
    bool texFlip;       ///< Whether the sprite is flipped horizontally.

    BulletManager
        *bulletManager; ///< Pointer to the bullet manager for ranged attacks.

  public:
    /**
     * @brief Constructs a Boss enemy.
     * @param texIdle   Pointer to the idle spritesheet.
     * @param texWalk   Pointer to the walk spritesheet.
     * @param texAttack Pointer to the attack spritesheet.
     * @param startPos  Initial world position.
     * @param bm        Pointer to the bullet manager.
     * @param am        Pointer to the audio manager.
     */
    Boss(Texture2D *texIdle, Texture2D *texWalk, Texture2D *texAttack,
         Vector2 startPos, BulletManager *bm, AudioManager *am);

    void Update(float dt, Vector2 playerPos, Map *map) override;
    void Draw() override;
    Rectangle GetHitbox() override;
    void DrawHealthBar() override;
};
