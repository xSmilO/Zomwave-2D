#include "Player.h"
#include "math.h"
#include "raylib.h"

Player::Player() {
    position = {0.0f, 0.0f};
    speed = 256.0f;
    playerWidth = 40.0f;
    playerHeight = 40.0f;

    // load Player animation
    std::vector<Vector2> idleFramePos = {{2, 2},  {7, 2},  {12, 2},
                                         {17, 2}, {22, 2}, {27, 2}};
    animator.AddAnimation("idle", "../assets/soldier-idle.png", 20, 2,
                          idleFramePos, true);
    animator.AddAnimation("walk", "../assets/soldier-walk.png", 20, 8,
                          idleFramePos, true);
    animator.SetState("idle");
}

void Player::Update(Map *map) {
    float dt = GetFrameTime();
    float moveX = 0.0f;
    float moveY = 0.0f;

    if (IsKeyDown(KEY_A)) {
        moveX -= speed * dt;
        facingLeft = true;
    }
    if (IsKeyDown(KEY_D)) {
        moveX += speed * dt;
        facingLeft = false;
    }
    if (IsKeyDown(KEY_W)) {
        moveY -= speed * dt;
    }
    if (IsKeyDown(KEY_S)) {
        moveY += speed * dt;
    }

    moveX = std::floor(moveX);
    moveY = std::floor(moveY);

    if (moveX != 0.0f) {
        Rectangle futureHitbox = {position.x + moveX, position.y, playerWidth,
                                   playerHeight};
        if (map->CheckHitbox(futureHitbox) == false) {
            position.x += moveX;
        }
    }

    if (moveY != 0.0f) {
        Rectangle futureHitbox = {position.x, position.y + moveY, playerWidth,
                                   playerHeight};
        if (map->CheckHitbox(futureHitbox) == false) {
            position.y += moveY;
        }
    }

    if (moveX != 0.0f || moveY != 0.0f)
        animator.SetState("walk");
    else
        animator.SetState("idle");

    animator.Update();
}

void Player::Draw() {
    DrawRectangle(position.x, position.y, 40, 40, RED);

    animator.Draw({position.x, position.y, playerWidth, playerHeight},
                  facingLeft);
}

void Player::SetPosition(Vector2 newPosition) { position = newPosition; }

Vector2 Player::GetPosition() { return position; }
