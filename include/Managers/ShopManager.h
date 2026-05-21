#pragma once

#include "Entities/Player.h"
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
    int activeTab = 0;

  public:
    ShopManager();

    bool isOpen = false;

    void UpdateAndDraw(Player *player, int screenWidth, int screenHeight);
    void ApplyUpgrade(Player *player, Upgrade &upgrade);
};
