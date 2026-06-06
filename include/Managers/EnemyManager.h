#pragma once
#include "BulletManager.h"
#include "Entities/Enemy.h"
#include "Entities/Player.h"
#include "Managers/CoinManager.h"
#include "Managers/GameBalance.h"
#include "Managers/ResourceManager.h"
#include "Map.h"
#include <memory>
#include <vector>

class EnemyManager {
  private:
    ResourceManager *resources;
    BulletManager *bulletManager;
    AudioManager *audioManager;

  public:
    EnemyManager(ResourceManager *rm, BulletManager *bm, AudioManager *am);

    std::vector<std::unique_ptr<Enemy>> enemies;

    void SpawnZombie(Vector2 pos, GameBalance *gm);
    void SpawnSkeleton(Vector2 pos, GameBalance *gm);

    void AddEnemy(std::unique_ptr<Enemy> enemy);

    void Update(float dt, Player *player, Map *map,
                BulletManager *bulletManager, CoinManager *coinManager);
    void Draw();

    std::unique_ptr<Enemy> CreateZombie(const GameBalance& gb);
    std::unique_ptr<Enemy> CreateSkeleton(const GameBalance& gb);
};
