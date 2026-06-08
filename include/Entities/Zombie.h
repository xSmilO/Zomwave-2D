#pragma once
#include "Animator.h"
#include "Entities/Enemy.h"

/**
 * @class Zombie
 * @brief A melee enemy that chases the player and attacks at close range.
 *
 * Inherits from Enemy. Uses a single walking animation and plays ambient
 * sounds at intervals.
 */
class Zombie : public Enemy {
  private:
    bool texFlip;       ///< Whether the sprite is flipped horizontally.
    Animator animator;  ///< Animator for the zombie walk animation.
    float ambientTimer; ///< Timer for ambient sound playback.

  public:
    /**
     * @brief Constructs a Zombie enemy.
     * @param texZombie Pointer to the zombie spritesheet texture.
     * @param startPos  Initial world position.
     * @param am        Pointer to the audio manager.
     */
    Zombie(Texture2D *texZombie, Vector2 startPos, AudioManager *am);

    void Update(float dt, Vector2 playerPos, Map *map) override;
    void Draw() override;
    Rectangle GetHitbox() override;
};
