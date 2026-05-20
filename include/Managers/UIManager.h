#pragma once

#include "Entities/Player.h"
#include "Managers/EnemyManager.h"
#include "Managers/WaveManager.h"
class UIManager {
  private:
      void DrawWave(WaveManager* waveManager, int screenWidth);
      void DrawHP(Player* player);
      void DrawTime(WaveManager* waveManager, int screenWidth);
      void DrawEnemies(EnemyManager* enemyManager, int screenWidth);
      
  public:
    UIManager();

    void DrawHUD(Player *player, WaveManager *waveManager,
                 EnemyManager *enemyManager, int screenWidth);
};
