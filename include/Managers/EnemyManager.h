#pragma once
#include "BulletManager.h"
#include "Entities/Enemy.h"
#include "Entities/Player.h"
#include "Managers/PickupManager.h"
#include "Managers/ResourceManager.h"
#include "Map.h"
#include <memory>
#include <vector>

class EnemyManager {
  private:
    ResourceManager *resources;
    BulletManager* bulletManager;

  public:
    EnemyManager(ResourceManager *resources, BulletManager* bm);

    std::vector<std::unique_ptr<Enemy>> enemies;

    void SpawnZombie(Vector2 pos);
    void SpawnSkeleton(Vector2 pos);

    void AddEnemy(std::unique_ptr<Enemy> enemy);

    void Update(float dt, Player *player, Map *map,
                BulletManager *bulletManager, PickupManager* pickupManager);
    void Draw();

    std::unique_ptr<Enemy> CreateZombie();
    std::unique_ptr<Enemy> CreateSkeleton();
};
