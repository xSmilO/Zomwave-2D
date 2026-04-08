#include "player.h"
#include "raylib.h"
#include "math.h"

Player::Player() {
  position = {0.0f, 0.0f};
  speed = 5.0f;
  radius = 20.0f;
  rotationAngle = 0.0f;
}

void Player::Update(Vector2 mouseWorldPos) {
  if(IsKeyDown(KEY_W)) position.y -= speed;
  if(IsKeyDown(KEY_S)) position.y += speed;
  if(IsKeyDown(KEY_A)) position.x -= speed;
  if(IsKeyDown(KEY_D)) position.x += speed;

  float dx = mouseWorldPos.x - position.x;
  float dy = mouseWorldPos.y - position.y;

  rotationAngle = atan2(dy, dx) * (180 / PI);
}

void Player::Draw() {
  Rectangle gunRect = {position.x, position.y, 35.0f, 8.0f};
  Vector2 gunOrigin = {0.0f, 4.0f};

  DrawCircleV(position, radius, BLUE);
  DrawRectanglePro(gunRect, gunOrigin, rotationAngle, DARKGRAY);
}

Vector2 Player::GetPosition() { return position;}
