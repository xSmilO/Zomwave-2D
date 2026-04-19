#pragma once
#include "Animator.h"
#include "Map.h"
#include "raylib.h"

class Player {
  private:
    Vector2 position;
    float speed;
    float radius;
    float rotationAngle;
    float playerWidth;
    float playerHeight;
    Animator animator;
    bool facingLeft;

  public:
    Player();
    void Update(Map* map);
    void Draw();
    void SetPosition(Vector2 newPosition);
    Vector2 GetPosition();
};
