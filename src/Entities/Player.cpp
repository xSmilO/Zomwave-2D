#include "Entities/Player.h"
#include "math.h"
#include "raylib.h"
#include "raymath.h"

Player::Player(Texture2D* playerIdle, Texture2D* playerWalk) {
    position = {0.0f, 0.0f};
    speed = 256.0f;
    playerWidth = 50.0f;
    playerHeight = 50.0f;
    weapon = nullptr;

    // load Player animation
    std::vector<Vector2> idleFramePos = {{2, 2},  {7, 2},  {12, 2},
                                         {17, 2}, {22, 2}, {27, 2}};
    std::vector<Vector2> walkFramePos = {
        {2, 2}, {7, 2}, {12, 2}, {17, 2}, {22, 2}, {27, 2}, {32, 2}, {37, 2},
    };

    animator.AddAnimation("idle", playerIdle, {20, 20}, 2,
                          idleFramePos, true);
    animator.AddAnimation("walk", playerWalk, {20, 20}, 12,
                          walkFramePos, true);

    animator.SetState("idle");
}

void Player::Update(Vector2 mousePosition, Map *map,
                    BulletManager *bulletManager) {
    float dt = GetFrameTime();

    Vector2 inputDir = {0.0f, 0.0f};

    if (IsKeyDown(KEY_A)) {
        inputDir.x -= 1.0f;
        facingLeft = true;
    }

    if (IsKeyDown(KEY_D)) {
        inputDir.x += 1.0f;
        facingLeft = false;
    }

    if (IsKeyDown(KEY_W)) {
        inputDir.y -= 1.0f;
    }

    if (IsKeyDown(KEY_S)) {
        inputDir.y += 1.0f;
    }

    if (inputDir.x != 0.0f || inputDir.y != 0.0f) {
        inputDir = Vector2Normalize(inputDir);
    }

    float moveX = inputDir.x * speed * dt;
    float moveY = inputDir.y * speed * dt;
    float hitX = position.x - (playerWidth / 2.0f);
    float hitY = position.y - (playerHeight / 2.0f);

    if (moveX != 0.0f) {
        futureHitbox = {hitX + moveX, hitY, playerWidth, playerHeight};
        if (map->CheckHitbox(futureHitbox) == false) {
            position.x += moveX;
        }
    }

    if (moveY != 0.0f) {
        futureHitbox = {hitX, hitY + moveY, playerWidth, playerHeight};
        if (map->CheckHitbox(futureHitbox) == false) {
            position.y += moveY;
        }
    }

    if (moveX != 0.0f || moveY != 0.0f)
        animator.SetState("walk");
    else
        animator.SetState("idle");

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (weapon != nullptr) {
            weapon->Shoot(mousePosition, bulletManager);
        }
    }

    CalculateWeaponPos(mousePosition);

    animator.Update();
    if (weapon != nullptr)
        weapon->Update(weaponPosition, weaponRotation, dt);
}

void Player::Draw() {
    // DrawRectangle(position.x - (playerWidth / 2.0f),
    //               position.y - (playerHeight / 2.0f), 40, 40, RED);

    animator.Draw({position.x, position.y, playerWidth, playerHeight},
                  facingLeft);

    // DrawRectangleRec(futureHitbox, RED);

    if (weapon != nullptr)
        weapon->Draw();

    // DrawCircleV({position.x, position.y}, 10.0f, RED);
}

void Player::SetPosition(Vector2 newPosition) { position = newPosition; }

Vector2 Player::GetPosition() { return position; }

void Player::SetWeapon(Pistol *pistol) { weapon = pistol; }

void Player::CalculateWeaponPos(Vector2 mousePosition) {
    Vector2 center = {position.x, position.y};

    float dx = mousePosition.x - center.x;
    float dy = mousePosition.y - center.y;
    float angleRad = atan2f(dy, dx);

    float orbitRadius = 50.0f;

    weaponPosition.x = center.x + cosf(angleRad) * orbitRadius;
    weaponPosition.y = center.y + sinf(angleRad) * orbitRadius;
    weaponRotation = angleRad * (180.0f / PI);
}
