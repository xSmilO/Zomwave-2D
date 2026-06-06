#include "Managers/PickupManager.h"
#include "Managers/AudioManager.h"
#include "raylib.h"
#include <cmath>

PickupManager::PickupManager(Texture2D *texHealthPotion, Texture2D *texCoin) {
    this->texHealthPotion = texHealthPotion;
    this->texCoin = texCoin;
}

void PickupManager::Spawn(Vector2 pos, PickupType type) {
    pickups.push_back({pos, type, 0.0f});
}

void PickupManager::Update(float dt, Player *player, AudioManager* audioManager) {
    for (auto it = pickups.begin(); it != pickups.end();) {
        it->floatTimer += dt * 5.0f;

        if (CheckCollisionRecs(it->GetHitbox(), player->GetHitbox())) {
            if (it->type == PickupType::COIN) {
                player->coins += 10;
                audioManager->PlayCoinPickup();
            }

            it = pickups.erase(it);
        } else {
            ++it;
        }
    }
}

void PickupManager::Draw() {
    for (auto &p : pickups) {
        float offsetY = sin(p.floatTimer) * 3.0f;
        Vector2 drawPos = {p.position.x - 8.0f, p.position.y - 8.0f + offsetY};

        if (p.type == PickupType::COIN) {
            DrawTextureV(*texCoin, drawPos, WHITE);
        }
    }
}
