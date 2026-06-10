#include "Entities/Player.h"
#include "Managers/BombManager.h"
#include "Managers/BulletManager.h"
#include "Managers/ResourceManager.h"
#include "Weapon.h"
#include "math.h"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <cmath>

Player::Player(ResourceManager *rm, AudioManager *am, GameBalance *gm) {
    resourceManager = rm;
    audioManager = am;
    gameBalance = gm;

    position = {0.0f, 0.0f};
    currentWeaponIndex = 0;

    width = 30.0f;
    height = 30.0f;
    timeBetweenSteps = 0.4f;
    stepTimer = 0.0f;

    maxHealth = 100.0f;
    health = maxHealth;
    invincibilityDuration = 0.3f;
    invincibilityTimer = 0.0;
    potions = 3;
    coins = 10000;
    shootTimer = 0.0f;

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

    InitStats();
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
        health += 40;
        audioManager->PlayPotionUse();

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

    if (moveX != 0.0f || moveY != 0.0f) {
        stepTimer -= dt;
        if (stepTimer <= 0.0f) {
            audioManager->PlayPlayerStep();
            stepTimer = timeBetweenSteps;
        }
    } else
        stepTimer = 0.0f;

    UpdateWeapon(dt, mousePosition, bulletManager);
}

void Player::Draw() {
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

    if (wp->type == WeaponType::SHOTGUN) {
        orbitRadius = 45.0f;
    }

    weaponPosition.x = center.x + cosf(angleRad) * orbitRadius;
    weaponPosition.y = center.y + sinf(angleRad) * orbitRadius;
    weaponRotation = angleRad * (180.0f / PI);
}

void Player::TakeDamage(float damage) {
    if (invincibilityTimer <= 0.0f) {
        health -= damage;
        audioManager->PlayPlayerHurt();
        invincibilityTimer = invincibilityDuration;

        if (health <= 0.0f) {

            if (potions > 0) {
                potions--;
                health = 40.0f;
                audioManager->PlayPotionUse();
            } else {
                health = 0.0f;
                isAlive = false;
            }
        }
    }
}

Rectangle Player::GetHitbox() {
    return {position.x - (width / 2), position.y - (height / 2) - 4, width,
            height};
}

void Player::InitializeArsenal() {
    std::vector<Vector2> shootFramePos = {};
    std::vector<Vector2> reloadFramePos = {};
    arsenal.push_back(std::make_unique<Weapon>());
    Weapon *glock = arsenal.back().get();
    glock->name = "Glock";
    glock->type = WeaponType::GLOCK;
    glock->width = 40;
    glock->height = 32;
    glock->currentLevel = 1;
    glock->isUnlocked = true;
    glock->barrelOffest = {15, -8};
    glock->LoadStatsFromBalance(*gameBalance);

    shootFramePos = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0},  {5, 0},
                     {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}};
    reloadFramePos = {{0, 0},  {1, 0},  {2, 0},  {3, 0}, {4, 0},  {5, 0},
                      {6, 0},  {7, 0},  {8, 0},  {9, 0}, {10, 0}, {11, 0},
                      {12, 0}, {13, 0}, {14, 0}, {15, 0}};

    std::reverse(reloadFramePos.begin(), reloadFramePos.end());

    glock->animator.AddAnimation("IDLE", &resourceManager->texGlockShoot,
                                 {64, 48}, 0, {{0, 0}}, false);
    glock->animator.AddAnimation("SHOOT", &resourceManager->texGlockShoot,
                                 {64, 48}, 42, shootFramePos, false);
    glock->animator.AddAnimation("RELOAD", &resourceManager->texGlockReload,
                                 {64, 48}, 24, reloadFramePos, false);

    glock->animator.SetState("IDLE");

    arsenal.push_back(std::make_unique<Weapon>());
    Weapon *mp5 = arsenal.back().get();
    mp5->name = "Mp5";
    mp5->type = WeaponType::MP5;
    mp5->width = 50;
    mp5->height = 34;
    mp5->isUnlocked = false;
    mp5->currentLevel = 1;
    mp5->barrelOffest = {16, -10};
    mp5->LoadStatsFromBalance(*gameBalance);
    shootFramePos = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0},  {5, 0},
                     {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}};
    reloadFramePos = {{0, 0},  {1, 0},  {2, 0},  {3, 0}, {4, 0},  {5, 0},
                      {6, 0},  {7, 0},  {8, 0},  {9, 0}, {10, 0}, {11, 0},
                      {12, 0}, {13, 0}, {14, 0}, {15, 0}};

    mp5->animator.AddAnimation("IDLE", &resourceManager->texMp5Shoot, {80, 48},
                               0, {{0, 0}}, false);
    mp5->animator.AddAnimation("SHOOT", &resourceManager->texMp5Shoot, {80, 48},
                               42, shootFramePos, false);
    mp5->animator.AddAnimation("RELOAD", &resourceManager->texMp5Reload,
                               {80, 48}, 24, reloadFramePos, false);

    mp5->animator.SetState("IDLE");

    arsenal.push_back(std::make_unique<Weapon>());
    Weapon *ak47 = arsenal.back().get();
    ak47->name = "Ak-47";
    ak47->type = WeaponType::AK47;
    ak47->width = 60;
    ak47->height = 36;
    ak47->isUnlocked = false;
    ak47->barrelOffest = {18, -10};
    ak47->LoadStatsFromBalance(*gameBalance);
    shootFramePos = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0},  {5, 0},
                     {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}};
    reloadFramePos = {{0, 0},  {1, 0},  {2, 0},  {3, 0}, {4, 0},  {5, 0},
                      {6, 0},  {7, 0},  {8, 0},  {9, 0}, {10, 0}, {11, 0},
                      {12, 0}, {13, 0}, {14, 0}, {15, 0}};

    ak47->animator.AddAnimation("IDLE", &resourceManager->texAk47Shoot,
                                {96, 48}, 0, {{0, 0}}, false);
    ak47->animator.AddAnimation("SHOOT", &resourceManager->texAk47Shoot,
                                {96, 48}, 42, shootFramePos, false);
    ak47->animator.AddAnimation("RELOAD", &resourceManager->texAk47Reload,
                                {96, 48}, 20, reloadFramePos, false);

    ak47->animator.SetState("IDLE");

    arsenal.push_back(std::make_unique<Weapon>());
    Weapon *shotgun = arsenal.back().get();
    shotgun->name = "Shotgun";
    shotgun->type = WeaponType::SHOTGUN;
    shotgun->width = 128;
    shotgun->height = 128;
    shotgun->isUnlocked = false;
    shotgun->barrelOffest = {32, -4};
    shotgun->LoadStatsFromBalance(*gameBalance);

    shootFramePos = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0},
                     {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}};
    reloadFramePos = {{10, 0}, {9, 0}, {8, 0}, {7, 0}, {6, 0}};

    std::reverse(reloadFramePos.begin(), reloadFramePos.end());

    shotgun->animator.AddAnimation("IDLE", &resourceManager->texShotgunShoot,
                                   {128, 128}, 0, {{0, 0}}, false);
    shotgun->animator.AddAnimation("SHOOT", &resourceManager->texShotgunShoot,
                                   {128, 128}, 42, shootFramePos, false);
    shotgun->animator.AddAnimation("RELOAD", &resourceManager->texShotgunShoot,
                                   {128, 128}, 8, reloadFramePos, false);

    shotgun->animator.SetState("IDLE");
    currentWeaponIndex = 0;
}

Weapon *Player::GetActiveWeapon() { return arsenal[currentWeaponIndex].get(); }

void Player::UpdateWeapon(float dt, Vector2 mousePos,
                          BulletManager *bulletManager) {
    CalculateWeaponPos(mousePos);
    Weapon *wp = GetActiveWeapon();
    wp->animator.Update(dt);

    if (shootTimer > 0.0f) {
        shootTimer -= dt;
    }

    bool triesToShoot =
        IsMouseButtonDown(MOUSE_LEFT_BUTTON) && shootTimer <= 0.0f;

    bool canShoot = wp->type == WeaponType::SHOTGUN ||
                    (wp->state != WeaponState::RELOADING);

    if (triesToShoot && wp->currentAmmo > 0 && canShoot) {
        float angleRad = weaponRotation * (PI / 180.0f);
        Vector2 localBarrel = wp->barrelOffest;
        Vector2 rotateOffset = Vector2Rotate(localBarrel, angleRad);
        Vector2 barrelEnd = Vector2Add(weaponPosition, rotateOffset);

        for (int i = 0; i < wp->pellets; ++i) {
            float randomOffset =
                GetRandomValue(-wp->spread * 10, wp->spread * 10) / 10.0f;
            float finalAngleRad = angleRad + (randomOffset * DEG2RAD);
            Vector2 finalDirection = {cosf(finalAngleRad), sinf(finalAngleRad)};
            Vector2 bulletTarget =
                Vector2Add(barrelEnd, Vector2Scale(finalDirection, 1000));

            bulletManager->Shoot(barrelEnd, bulletTarget, wp->damage,
                                 BulletType::BULLET, 800, randomOffset, false);
        }

        wp->currentAmmo--;
        shootTimer = wp->fireCooldown;

        wp->state = WeaponState::SHOOTING;
        wp->animator.SetState("SHOOT");
        wp->animator.ResetAnimation();

        audioManager->PlayShoot(wp->type);
    } else if (wp->state == WeaponState::RELOADING) {
        if (wp->animator.IsAnimationFinished()) {

            if (wp->type == WeaponType::SHOTGUN) {
                wp->currentAmmo++;

                if (wp->currentAmmo < wp->maxAmmo) {
                    wp->animator.ResetAnimation();
                    audioManager->PlayReload(wp->type);
                } else {
                    wp->state = WeaponState::IDLE;
                    wp->animator.SetState("IDLE");
                }
            } else {
                wp->currentAmmo = wp->maxAmmo;
                wp->state = WeaponState::IDLE;
                wp->animator.SetState("IDLE");
            }
        }
    } else {
        if ((IsKeyPressed(KEY_R) && wp->currentAmmo < wp->maxAmmo) ||
            (triesToShoot && wp->currentAmmo == 0)) {

            wp->state = WeaponState::RELOADING;
            wp->animator.SetState("RELOAD");
            wp->animator.ResetAnimation();
            audioManager->PlayReload(wp->type);

        } else if (wp->state == WeaponState::SHOOTING) {
            if (wp->animator.IsAnimationFinished()) {
                wp->state = WeaponState::IDLE;
                wp->animator.SetState("IDLE");
            }
        }
    }
}

void Player::EquipWeapon(WeaponType type) {
    for (size_t i = 0; i < arsenal.size(); ++i) {
        if (arsenal[i]->type == type) {
            currentWeaponIndex = i;
            GetActiveWeapon()->animator.SetState("IDLE");
            break;
        }
    }
}

Weapon *Player::GetWeapon(WeaponType type) {
    for (size_t i = 0; i < arsenal.size(); ++i) {
        if (arsenal[i]->type == type)
            return arsenal[i].get();
    }

    return nullptr;
}

void Player::InitStats() {
    this->coins = gameBalance->player.coins;

    this->maxHealthLevel = 0;
    this->maxHealth = gameBalance->player.maxHealthLevels[0];
    this->health = this->maxHealth;

    this->speedLevel = 0;
    this->speed = gameBalance->player.speedLevels[0];

    this->potionsBought = 0;
    this->visionLevel = 0;
    this->visionRadius = gameBalance->player.visionLevels[0];
    this->bombs = gameBalance->player.bombs;
    this->potions = gameBalance->player.potions;
    this->invincibilityDuration = gameBalance->player.invincibilityDuration;

    currentWeaponIndex = 0;
}
