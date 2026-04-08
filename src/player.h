#pragma once
#include "raylib.h"

class Player {
  private:
    Vector2 position;
    float speed;
    float radius;
    float rotationAngle;


  public:
    Player();
    void Update(Vector2 mouseWorldPos);
    void Draw();
    Vector2 GetPosition();
};
