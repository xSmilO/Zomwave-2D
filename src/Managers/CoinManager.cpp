#include "Managers/CoinManager.h"
#include "raymath.h"

void CoinManager::SpawnCoin(Vector2 pos, int value) {
    if (value <= 0)
        return;

    pos.x += GetRandomValue(-15, 15);
    pos.y += GetRandomValue(-15, 15);

    coins.push_back({pos, value, true});
}

void CoinManager::Update(float dt, Player *player, AudioManager *audioManager) {
    for (auto &coin : coins) {
        if (!coin.active)
            continue;

        float dist = Vector2Distance(coin.position, player->GetPosition());

        if (dist <= pickupRadius) {
            coin.active = false;
            player->coins += coin.value;
            audioManager->PlayCoinPickup();
        }
        // 2. EFEKT MAGNESU
        else if (dist <= magnetRadius) {
            float speedBoost = 1.0f + (1.0f - (dist / magnetRadius));

            coin.position =
                Vector2MoveTowards(coin.position, player->GetPosition(),
                                   magnetSpeed * speedBoost * dt);
        }
    }

    coins.erase(std::remove_if(coins.begin(), coins.end(),
                               [](const Coin &c) { return !c.active; }),
                coins.end());
}

void CoinManager::Draw(Texture2D coinTexture) {
    for (const auto &coin : coins) {
        float texW = (float)coinTexture.width;
        float texH = (float)coinTexture.height;

        Vector2 drawPos = {coin.position.x - texW / 2.0f,
                           coin.position.y - texH / 2.0f};

        DrawTextureV(coinTexture, drawPos, WHITE);
    }
}
