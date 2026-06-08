#pragma once

#include "Entities/Player.h"
#include "Managers/GameBalance.h"
#include "Managers/ResourceManager.h"
#include <string>
#include <vector>

/**
 * @enum UpgradeType
 * @brief Types of player upgrades available in the shop.
 */
enum class UpgradeType {
    MAX_HP,
    SPEED
};

/**
 * @struct Upgrade
 * @brief Represents a single purchasable upgrade in the shop.
 */
struct Upgrade {
    std::string name;        ///< Display name of the upgrade.
    std::string description; ///< Description of what the upgrade does.
    UpgradeType type;        ///< Type of upgrade.

    int currentLevel = 0; ///< Current upgrade level.
    int maxLevel = 5;     ///< Maximum upgrade level.
    int baseCost = 50;    ///< Base cost before level scaling.

    /**
     * @brief Calculates the current cost based on level.
     * @return The cost in coins.
     */
    int GetCurrentCost() const {
        return baseCost + (baseCost * currentLevel * 0.5f);
    }
};

/**
 * @class ShopManager
 * @brief Manages the in-game shop UI and upgrade logic.
 *
 * Allows the player to purchase health and speed upgrades, buy potions,
 * and view upgrade progress. Renders a windowed shop interface.
 */
class ShopManager {
  private:
    std::vector<Upgrade> upgrades;                ///< List of available upgrades.
    int currentTab = 0;                           ///< Currently selected shop tab.
    Vector2 scrollOffset = {0, 0};                ///< Scroll offset for the shop window.
    ResourceManager *resourceManager;             ///< Pointer to the resource manager.
    Rectangle windowBounds = {100, 25, 600, 400}; ///< Shop window rectangle.
    int potionCost = 50;                          ///< Current cost of a health potion.
    int potionsBought = 0;                        ///< Total potions purchased this run.

    /// Recalculates potion and upgrade prices based on purchases and levels.
    void CalculateNewPrices();

  public:
    ShopManager();

    bool isOpen = false; ///< Whether the shop window is currently open.

    /**
     * @brief Draws the shop UI and handles purchase interactions.
     * @param player         Pointer to the player (for applying upgrades).
     * @param resourceManager Pointer to the resource manager (for textures).
     * @param audioManager   Pointer to the audio manager (for sounds).
     * @param balance        Reference to the game balance config.
     */
    void DrawShop(Player *player, ResourceManager *resourceManager, AudioManager *audioManager,
                  const GameBalance &balance);
};
