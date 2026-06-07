#pragma once

#include "Entities/Player.h"
#include "raylib.h"
#include <vector>

struct Coin {
    Vector2 position;
    int value;
    bool active = true;
};

class CoinManager {
private:
    float magnetRadius = 50.0f;
    float magnetSpeed = 400.0f;
    float pickupRadius = 25.0f; 

public:
    std::vector<Coin> coins;

    void SpawnCoin(Vector2 pos, int value);
    void Update(float dt, Player* player, AudioManager* audioManager);
    void Draw(Texture2D coinTexture);
    void Clear();
};
