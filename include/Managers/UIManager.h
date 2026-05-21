#pragma once

#include "Entities/Player.h"
#include "Managers/EnemyManager.h"
#include "Managers/WaveManager.h"
class UIManager {
  private:
    Texture2D *texHealthPotion;
    Texture2D *texCoin;

    void DrawWave(WaveManager *waveManager, int screenWidth);
    void DrawHP(Player *player);
    void DrawTime(WaveManager *waveManager, int screenWidth);
    void DrawEnemies(EnemyManager *enemyManager, int screenWidth);
    void DrawLootInfo(Player *player);

  public:
    UIManager(Texture2D* texHealthPotion, Texture2D* texCoin);

    void DrawHUD(Player *player, WaveManager *waveManager,
                 EnemyManager *enemyManager, int screenWidth);
};
