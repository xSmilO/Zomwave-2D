#pragma once

#include "Entities/Player.h"
#include "Managers/EnemyManager.h"
#include "Managers/WaveManager.h"

/**
 * @class UIManager
 * @brief Renders the in-game HUD (heads-up display).
 *
 * Draws wave info, health bar, timer, enemy count, loot info,
 * and ammunition display on screen.
 */
class UIManager {
  private:
    Texture2D *texHealthPotion; ///< Texture for the potion icon in HUD.
    Texture2D *texCoin;         ///< Texture for the coin icon in HUD.
    Texture2D *texBomb;         ///< Texture for the bomb icon in HUD.

    /**
     * @brief Draws the current wave number and wave timer.
     * @param waveManager Pointer to the wave manager.
     * @param screenWidth Current screen width.
     */
    void DrawWave(WaveManager *waveManager, int screenWidth);

    /**
     * @brief Draws the player's health bar.
     * @param player Pointer to the player.
     */
    void DrawHP(Player *player);

    /**
     * @brief Draws the elapsed game time.
     * @param waveManager Pointer to the wave manager.
     * @param screenWidth Current screen width.
     */
    void DrawTime(WaveManager *waveManager, int screenWidth);

    /**
     * @brief Draws the remaining enemy count.
     * @param enemyManager Pointer to the enemy manager.
     * @param screenWidth  Current screen width.
     */
    void DrawEnemies(EnemyManager *enemyManager, int screenWidth);

    /**
     * @brief Draws coin, potion, and bomb counts.
     * @param player Pointer to the player.
     */
    void DrawLootInfo(Player *player);

    /**
     * @brief Draws the current weapon's ammo display.
     * @param player       Pointer to the player.
     * @param screenWidth  Current screen width.
     * @param screenHeight Current screen height.
     */
    void DrawAmmunition(Player *player, int screenWidth, int screenHeight);

  public:
    /**
     * @brief Constructs the UIManager with icon textures.
     * @param texHealthPotion Pointer to the health potion texture.
     * @param texCoin         Pointer to the coin texture.
     * @param texBomb         Pointer to the bomb texture.
     */
    UIManager(Texture2D *texHealthPotion, Texture2D *texCoin, Texture2D *texBomb);

    /**
     * @brief Draws the complete HUD overlay.
     * @param player       Pointer to the player.
     * @param waveManager  Pointer to the wave manager.
     * @param enemyManager Pointer to the enemy manager.
     * @param screenWidth  Current screen width.
     * @param screenHeight Current screen height.
     */
    void DrawHUD(Player *player, WaveManager *waveManager, EnemyManager *enemyManager, int screenWidth,
                 int screenHeight);
};
