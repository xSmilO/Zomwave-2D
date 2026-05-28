#pragma once

#include "Entities/Player.h"
#include "Managers/ResourceManager.h"
#include <string>
#include <vector>
enum class UpgradeType { MAX_HP, SPEED };

struct Upgrade {
    std::string name;
    std::string description;
    UpgradeType type;

    int currentLevel = 0;
    int maxLevel = 5;
    int baseCost = 50;

    int GetCurrentCost() const {
        return baseCost + (baseCost * currentLevel * 0.5f);
    }
};

class ShopManager {
  private:
    std::vector<Upgrade> upgrades;
    int currentTab = 0;
    Vector2 scrollOffset = {0, 0};
    ResourceManager* resourceManager;
    Rectangle windowBounds = {100, 25, 600, 400};

  public:
    ShopManager();

    bool isOpen = false;

    void DrawShop(Player *player, ResourceManager* resourceManager);
    void ApplyUpgrade(Player *player, Upgrade &upgrade);
};
