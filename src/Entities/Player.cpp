#include "Entities/Player.h"
#include "Managers/BulletManager.h"
#include "Managers/ResourceManager.h"
#include "Weapon.h"
#include "math.h"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <cmath>

Player::Player(ResourceManager *resourceManager) {
    this->resourceManager = resourceManager;
    position = {0.0f, 0.0f};

    speed = 256.0f;
    width = 30.0f;
    height = 30.0f;

    maxHealth = 100.0f;
    health = maxHealth;
    invincibilityDuration = 0.3f;
    potions = 3;
    coins = 0;

    // load Player animation
    std::vector<Vector2> idleFramePos = {{2, 2},  {7, 2},  {12, 2},
                                         {17, 2}, {22, 2}, {27, 2}};
    std::vector<Vector2> walkFramePos = {
        {2, 2}, {7, 2}, {12, 2}, {17, 2}, {22, 2}, {27, 2}, {32, 2}, {37, 2},
    };

    animator.AddAnimation("idle", &resourceManager->texPlayerIdle, {20, 20}, 2,
                          idleFramePos, true);
    animator.AddAnimation("walk", &resourceManager->texPlayerWalk, {20, 20}, 12,
                          walkFramePos, true);

    animator.SetState("idle");

    InitializeArsenal();
}

void Player::Update(float dt, Vector2 mousePosition, Map *map,
                    BulletManager *bulletManager) {
    invincibilityTimer -= dt;

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

    if (IsKeyPressed(KEY_E) && potions > 0 && health < maxHealth) {
        potions--;
        health += 30;

        if (health > maxHealth) {
            health = maxHealth;
        }
    }

    float moveX = inputDir.x * speed * dt;
    float moveY = inputDir.y * speed * dt;
    float hitX = position.x - (width / 2.0f);
    float hitY = position.y - (height / 2.0f);

    if (moveX != 0.0f) {
        futureHitbox = {hitX + moveX, hitY, width, height - 4};
        if (map->CheckHitbox(futureHitbox) == false) {
            position.x += moveX;
        }
    }

    if (moveY != 0.0f) {
        futureHitbox = {hitX, hitY + moveY, width, height - 4};
        if (map->CheckHitbox(futureHitbox) == false) {
            position.y += moveY;
        }
    }

    if (moveX != 0.0f || moveY != 0.0f)
        animator.SetState("walk");
    else
        animator.SetState("idle");

    animator.Update(dt);

    UpdateWeapon(dt, mousePosition, bulletManager);
}

void Player::Draw() {
    // DrawRectangleRec(GetHitbox(), BLUE);
    // DrawRectangleRec(futureHitbox, YELLOW);
    if (invincibilityTimer > 0.0f) {
        int blink = (int)(invincibilityTimer * 20.0f);
        if (blink % 2 == 0)
            return;
    }
    animator.Draw({position.x, position.y, width, height}, facingLeft);

    Weapon *wp = GetActiveWeapon();

    wp->animator.Draw({weaponPosition.x, weaponPosition.y, (float)wp->width,
                       (float)wp->height},
                      false, weaponRotation);
}

void Player::SetPosition(Vector2 newPosition) { position = newPosition; }

Vector2 Player::GetPosition() { return position; }

void Player::CalculateWeaponPos(Vector2 mousePosition) {
    Weapon *wp = GetActiveWeapon();
    Vector2 center = {position.x, position.y};

    float dx = mousePosition.x - center.x;
    float dy = mousePosition.y - center.y;
    float angleRad = atan2f(dy, dx);

    float orbitRadius = (width / 3.0f) + (wp->width / 2.0f);

    weaponPosition.x = center.x + cosf(angleRad) * orbitRadius;
    weaponPosition.y = center.y + sinf(angleRad) * orbitRadius;
    weaponRotation = angleRad * (180.0f / PI);
}

void Player::TakeDamage(float damage) {
    if (invincibilityTimer <= 0.0f) {
        health -= damage;
        invincibilityTimer = invincibilityDuration;
    }

    if (health < 0) {
        health = maxHealth;
    }
}

Rectangle Player::GetHitbox() {
    return {position.x - (width / 2), position.y - (height / 2) - 4, width,
            height};
}

void Player::InitializeArsenal() {
    Weapon pistol;
    pistol.name = "Glock-18";
    pistol.type = WeaponType::PISTOL;
    pistol.width = 40;
    pistol.height = 32;
    pistol.maxAmmo = 12;
    pistol.currentAmmo = pistol.maxAmmo;
    pistol.damage = 10;
    pistol.isUnlocked = true;
    pistol.reloadTime = 1.5f;
    pistol.fireCooldown = 0.45f;
    pistol.upgradeCost = 50;
    pistol.currentLevel = 1;
    pistol.maxLevel = 5;
    pistol.barrelOffest = {15, -8};
    std::vector<Vector2> shootFramePos = {{0, 0}, {1, 0}, {2, 0},  {3, 0},
                                          {4, 0}, {5, 0}, {6, 0},  {7, 0},
                                          {8, 0}, {9, 0}, {10, 0}, {11, 0}};
    std::vector<Vector2> reloadFramePos = {
        {0, 0}, {1, 0}, {2, 0},  {3, 0},  {4, 0},  {5, 0},  {6, 0},  {7, 0},
        {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}};

    std::reverse(reloadFramePos.begin(), reloadFramePos.end());

    pistol.animator.AddAnimation("IDLE", &resourceManager->texPistolShoot,
                                 {64, 48}, 0, {{0, 0}}, false);
    pistol.animator.AddAnimation("SHOOT", &resourceManager->texPistolShoot,
                                 {64, 48}, 42, shootFramePos, false);
    pistol.animator.AddAnimation("RELOAD", &resourceManager->texPistolReload,
                                 {64, 48}, 24, reloadFramePos, false);

    pistol.animator.SetState("IDLE");
    arsenal.push_back(pistol);

    Weapon mp5;
    mp5.name = "Mp5";
    mp5.type = WeaponType::SMG;
    mp5.width = 50;
    mp5.height = 34;
    mp5.maxAmmo = 25;
    mp5.currentAmmo = mp5.maxAmmo;
    mp5.damage = 8;
    mp5.spread = 1.5f;
    mp5.isUnlocked = false;
    mp5.reloadTime = 1.5f;
    mp5.fireCooldown = 0.1f;
    mp5.unlockCost = 500;
    mp5.upgradeCost = 150;
    mp5.currentLevel = 1;
    mp5.maxLevel = 8;
    mp5.barrelOffest = {16, -10};
    shootFramePos = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0},  {5, 0},
                     {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}};
    reloadFramePos = {{0, 0},  {1, 0},  {2, 0},  {3, 0}, {4, 0},  {5, 0},
                      {6, 0},  {7, 0},  {8, 0},  {9, 0}, {10, 0}, {11, 0},
                      {12, 0}, {13, 0}, {14, 0}, {15, 0}};

    // std::reverse(reloadFramePos.begin(), reloadFramePos.end());

    mp5.animator.AddAnimation("IDLE", &resourceManager->texMp5Shoot, {80, 48},
                              0, {{0, 0}}, false);
    mp5.animator.AddAnimation("SHOOT", &resourceManager->texMp5Shoot, {80, 48},
                              42, shootFramePos, false);
    mp5.animator.AddAnimation("RELOAD", &resourceManager->texMp5Reload,
                              {80, 48}, 24, reloadFramePos, false);

    mp5.animator.SetState("IDLE");
    arsenal.push_back(mp5);

    Weapon ak47;
    ak47.name = "Ak-47";
    ak47.type = WeaponType::ASSAULT_RIFLE;
    ak47.width = 60;
    ak47.height = 36;
    ak47.maxAmmo = 31;
    ak47.currentAmmo = ak47.maxAmmo;
    ak47.damage = 8;
    ak47.spread = 1.5f;
    ak47.isUnlocked = false;
    ak47.reloadTime = 1.5f;
    ak47.fireCooldown = 0.15f;
    ak47.upgradeCost = 200;
    ak47.unlockCost = 1500;
    ak47.currentLevel = 1;
    ak47.maxLevel = 10;
    ak47.barrelOffest = {18, -10};
    shootFramePos = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0},  {5, 0},
                     {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}};
    reloadFramePos = {{0, 0},  {1, 0},  {2, 0},  {3, 0}, {4, 0},  {5, 0},
                      {6, 0},  {7, 0},  {8, 0},  {9, 0}, {10, 0}, {11, 0},
                      {12, 0}, {13, 0}, {14, 0}, {15, 0}};

    // std::reverse(reloadFramePos.begin(), reloadFramePos.end());

    ak47.animator.AddAnimation("IDLE", &resourceManager->texAk47Shoot, {96, 48},
                               0, {{0, 0}}, false);
    ak47.animator.AddAnimation("SHOOT", &resourceManager->texAk47Shoot,
                               {96, 48}, 42, shootFramePos, false);
    ak47.animator.AddAnimation("RELOAD", &resourceManager->texAk47Reload,
                               {96, 48}, 20, reloadFramePos, false);

    ak47.animator.SetState("IDLE");
    arsenal.push_back(ak47);

    currentWeaponIndex = 0;
}

Weapon *Player::GetActiveWeapon() { return &arsenal[currentWeaponIndex]; }

void Player::UpdateWeapon(float dt, Vector2 mousePos,
                          BulletManager *bulletManager) {
    CalculateWeaponPos(mousePos);

    Weapon *wp = GetActiveWeapon();

    wp->animator.Update(dt);

    if (shootTimer > 0.0f) {
        shootTimer -= dt;
    }

    if (wp->state == WeaponState::RELOADING) {
        if (wp->animator.IsAnimationFinished()) {
            printf("kunic ladowania\n");
            wp->currentAmmo = wp->maxAmmo;
            wp->state = WeaponState::IDLE;
            wp->animator.SetState("IDLE");
        }
    } else {
        if (IsKeyPressed(KEY_R) && wp->currentAmmo < wp->maxAmmo) {
            wp->state = WeaponState::RELOADING;
            wp->animator.SetState("RELOAD");
            wp->animator.ResetAnimation();
        } else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && shootTimer <= 0.0f) {
            if (wp->currentAmmo > 0) {
                float angleRad = weaponRotation * (PI / 180.0f);
                Vector2 localBarrel = wp->barrelOffest;

                Vector2 rotateOffset = Vector2Rotate(localBarrel, angleRad);

                Vector2 barrelEnd = Vector2Add(weaponPosition, rotateOffset);

                for (int i = 0; i < wp->pellets; ++i) {
                    float randomOffset =
                        GetRandomValue(-wp->spread * 10, wp->spread * 10) /
                        10.0f;

                    float finalAngleRad = angleRad + (randomOffset * DEG2RAD);
                    Vector2 finalDirection = {cosf(finalAngleRad),
                                              sinf(finalAngleRad)};
                    Vector2 bulletTarget = Vector2Add(
                        barrelEnd, Vector2Scale(finalDirection, 1000));

                    bulletManager->Shoot(barrelEnd, bulletTarget, wp->damage,
                                         BulletType::BULLET, 800, randomOffset,
                                         false);
                }

                wp->currentAmmo--;
                shootTimer = wp->fireCooldown;
                wp->state = WeaponState::SHOOTING;
                wp->animator.SetState("SHOOT");
                wp->animator.ResetAnimation();
            } else {
                wp->state = WeaponState::RELOADING;
                wp->animator.SetState("RELOAD");
                wp->animator.ResetAnimation();
            }
        } else if (wp->state == WeaponState::SHOOTING) {
            if (wp->animator.IsAnimationFinished()) {
                wp->state = WeaponState::IDLE;
                wp->animator.SetState("IDLE");
            }
        }
    }
}

void Player::EquipWeapon(WeaponType type) {
    for (int i = 0; i < arsenal.size(); ++i) {
        if (arsenal[i].type == type) {
            currentWeaponIndex = i;
            GetActiveWeapon()->animator.SetState("IDLE");
            break;
        }
    }
}

Weapon* Player::GetWeapon(WeaponType type) {
    for (int i = 0; i < arsenal.size(); ++i) {
        if (arsenal[i].type == type) return &arsenal[i];
        
    }

    return nullptr;
}
