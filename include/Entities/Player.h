#pragma once
#include "Animator.h"
#include "Managers/AudioManager.h"
// #include "Managers/BombManager.h"
#include "Managers/BulletManager.h"
#include "Managers/GameBalance.h"
#include "Managers/ResourceManager.h"
#include "Map.h"
#include "Weapon.h"
#include "raylib.h"

/**
 * @class Player
 * @brief Represents the player character.
 *
 * Handles movement, weapon management, health, coins, potions, bombs,
 * and interactions with the game world. The player has an arsenal of
 * weapons that can be switched and upgraded.
 */
class Player {
  private:
    Vector2 position;                             ///< World position of the player.
    Vector2 weaponPosition;                       ///< Calculated position of the weapon sprite.
    float weaponRotation;                         ///< Rotation angle of the weapon towards the mouse.
    float radius;                                 ///< Collision radius.
    float rotationAngle;                          ///< Player sprite rotation angle.
    float width;                                  ///< Sprite width in pixels.
    float height;                                 ///< Sprite height in pixels.
    Animator animator;                            ///< Animator for player idle/walk/death animations.
    bool facingLeft;                              ///< Whether the player sprite is flipped horizontally.
    Rectangle futureHitbox;                       ///< Predicted hitbox for next-frame collision checks.
    std::vector<std::unique_ptr<Weapon>> arsenal; ///< Collection of owned weapons.
    int currentWeaponIndex;                       ///< Index of the currently equipped weapon.
    float shootTimer;                             ///< Accumulated time since last shot.
    float stepTimer;                              ///< Accumulated time for footstep sound timing.
    float timeBetweenSteps;                       ///< Time interval between footstep sounds.
    ResourceManager *resourceManager;             ///< Pointer to the resource manager.
    AudioManager *audioManager;                   ///< Pointer to the audio manager.
    GameBalance *gameBalance;                     ///< Pointer to the game balance config.
    // BombManager *bombManager;

    /**
     * @brief Calculates the weapon sprite position and rotation based on mouse aim.
     * @param mousePosition Target position in world coordinates.
     */
    void CalculateWeaponPos(Vector2 mousePosition);

    /// Initializes the player's weapon arsenal with default weapons.
    void InitializeArsenal();

  public:
    float invincibilityDuration; ///< Duration of invincibility after taking damage (seconds).
    float invincibilityTimer;    ///< Remaining invincibility time.
    float speed;                 ///< Movement speed in world units per second.
    float maxHealth;             ///< Maximum health points.
    float health;                ///< Current health points.

    int coins;   ///< Current coin count.
    int potions; ///< Number of health potions held.
    int bombs;   ///< Number of bombs held.

    bool isAlive; ///< Whether the player is alive.

    int maxHealthLevel; ///< Current max-health upgrade level.
    int speedLevel;     ///< Current speed upgrade level.
    int potionsBought;  ///< Total potions purchased (for pricing).
    int visionLevel;    ///< Current vision-radius upgrade level.
    float visionRadius; ///< Current vision radius in world units.

    /**
     * @brief Constructs the player and initialises stats and arsenal.
     * @param rm Pointer to the resource manager.
     * @param am Pointer to the audio manager.
     * @param gm Pointer to the game balance configuration.
     */
    Player(ResourceManager *rm, AudioManager *am, GameBalance *gm);

    /**
     * @brief Updates player logic: movement, weapon aiming, shooting, collisions.
     * @param dt             Delta time in seconds.
     * @param mousePosition  Mouse position in world coordinates.
     * @param map            Pointer to the map for collision checks.
     * @param bulletManager  Pointer to the bullet manager for shooting.
     */
    void Update(float dt, Vector2 mousePosition, Map *map, BulletManager *bulletManager);

    /// Draws the player and the currently equipped weapon.
    void Draw();

    /**
     * @brief Teleports the player to a new position.
     * @param newPosition The new world position.
     */
    void SetPosition(Vector2 newPosition);

    /**
     * @brief Updates the weapon state (shooting, reloading, aiming).
     * @param dt             Delta time in seconds.
     * @param mousePos       Mouse position in world coordinates.
     * @param bulletManager  Pointer to the bullet manager.
     */
    void UpdateWeapon(float dt, Vector2 mousePos, BulletManager *bulletManager);

    /**
     * @brief Applies damage to the player (respects invincibility frames).
     * @param damage Amount of damage to subtract.
     */
    void TakeDamage(float damage);

    /// @return The player's world position.
    Vector2 GetPosition();

    /// @return The player's collision rectangle.
    Rectangle GetHitbox();

    /// @return Pointer to the currently equipped weapon.
    Weapon *GetActiveWeapon();

    /**
     * @brief Returns a weapon of the given type from the arsenal.
     * @param type The weapon type to find.
     * @return Pointer to the weapon, or nullptr if not owned.
     */
    Weapon *GetWeapon(WeaponType type);

    /**
     * @brief Equips a weapon of the given type.
     * @param type The weapon type to equip.
     */
    void EquipWeapon(WeaponType type);

    /// Upgrades the currently equipped weapon to the next level.
    void UpgradeWeapon();

    /// Initialises or resets all player stats from the game balance config.
    void InitStats();
};
