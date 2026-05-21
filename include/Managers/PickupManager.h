#pragma once

#include "Entities/Player.h"
#include "raylib.h"
#include <vector>
enum class PickupType { COIN, POTION };

struct Pickup {
  Vector2 position;
  PickupType type;
  float floatTimer = 0.0f;

  Rectangle GetHitbox() {
    return {position.x - 8.0f, position.y - 8.0f, 16.0f, 16.0f};
  }
};

class PickupManager {
  private:
    std::vector<Pickup> pickups;

    Texture2D* texCoin;
    Texture2D* texHealthPotion;
  
  public:
    PickupManager(Texture* texHealthPotion, Texture* texCoin);
    void Spawn(Vector2 pos, PickupType type);
    void Update(float dt, Player* player);
    void Draw();
};
