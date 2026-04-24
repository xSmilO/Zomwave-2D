#pragma once
#include "Animator.h"
#include "Map.h"
#include "Pistol.h"
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

    void CalculateWeaponPos(Vector2 mousePosition);

  public:
    Player();
    void Update(Vector2 mousePosition, Map *map);
    void Draw();
    void SetPosition(Vector2 newPosition);
    void SetWeapon(Pistol *pistol);
    Vector2 GetPosition();
};
