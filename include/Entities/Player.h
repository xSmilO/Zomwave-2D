#pragma once
#include "Animator.h"
#include "Managers/BulletManager.h"
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
    float invincibilityDuration;
    std::vector<Weapon> arsenal;
    int currentWeaponIndex;
    float shootTimer;
    ResourceManager *resourceManager;

    void CalculateWeaponPos(Vector2 mousePosition);
    void InitializeArsenal();

  public:
    float speed;
    float maxHealth;
    float health;
    float invincibilityTimer;
    int coins;
    int potions;

    Player(ResourceManager *resourceManager);
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
};
