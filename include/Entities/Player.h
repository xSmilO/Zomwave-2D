#pragma once
#include "Animator.h"
#include "Managers/BulletManager.h"
#include "Map.h"
#include "Weapons/Pistol.h"
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
    Pistol *weapon;
    Rectangle futureHitbox;
    float invincibilityDuration;

    void CalculateWeaponPos(Vector2 mousePosition);

  public:
    float speed;
    float maxHealth;
    float health;
    float invincibilityTimer;
    int coins;
    int potions;

    Player(Texture2D *playerIdle, Texture2D *playerWalk);
    void Update(float dt, Vector2 mousePosition, Map *map, BulletManager *bulletManager);
    void Draw();
    void SetPosition(Vector2 newPosition);
    void SetWeapon(Pistol *pistol);
    Vector2 GetPosition();
    void TakeDamage(float damage);
    Rectangle GetHitbox();
};
