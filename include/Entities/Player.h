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
    float speed;
    float radius;
    float rotationAngle;
    float playerWidth;
    float playerHeight;
    Animator animator;
    bool facingLeft;
    Pistol *weapon;
    Rectangle futureHitbox;

    void CalculateWeaponPos(Vector2 mousePosition);

  public:
    Player(Texture2D* playerIdle, Texture2D* playerWalk);
    void Update(Vector2 mousePosition, Map *map, BulletManager* bulletManager);
    void Draw();
    void SetPosition(Vector2 newPosition);
    void SetWeapon(Pistol *pistol);
    Vector2 GetPosition();
};
