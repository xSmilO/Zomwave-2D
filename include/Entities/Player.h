#pragma once
#include "Animator.h"
#include "Managers/AudioManager.h"
#include "Managers/BulletManager.h"
#include "Managers/GameBalance.h"
#include "Managers/ResourceManager.h"
#include "Map.h"
#include "Weapon.h"
#include "raylib.h"

class Player {
  private:
    Vector2 position;
    Vector2 weaponPosition;
    float weaponRotation;
    float radius;
    float rotationAngle;
    float width;
    float height;
    Animator animator;
    bool facingLeft;
    Rectangle futureHitbox;
    std::vector<std::unique_ptr<Weapon>> arsenal;
    int currentWeaponIndex;
    float shootTimer;
    float stepTimer;
    float timeBetweenSteps;
    ResourceManager *resourceManager;
    AudioManager *audioManager;
    GameBalance *gameBalance;

    void CalculateWeaponPos(Vector2 mousePosition);
    void InitializeArsenal();

  public:
    float invincibilityDuration;
    float invincibilityTimer;
    float speed;
    float maxHealth;
    float health;

    int coins;
    int potions;

    bool isAlive;

    int maxHealthLevel;
    int speedLevel;
    int potionsBought;
    int visionLevel;
    float visionRadius;

    Player(ResourceManager *resourceManager, AudioManager *audioManager, GameBalance* gm);
    void Update(float dt, Vector2 mousePosition, Map *map,
                BulletManager *bulletManager);
    void Draw();
    void SetPosition(Vector2 newPosition);
    void UpdateWeapon(float dt, Vector2 mousePos, BulletManager *bulletManager);
    void TakeDamage(float damage);
    Vector2 GetPosition();
    Rectangle GetHitbox();
    Weapon *GetActiveWeapon();
    Weapon *GetWeapon(WeaponType type);
    void EquipWeapon(WeaponType type);
    void UpgradeWeapon();
    void InitStats();
};
