#pragma once
#include "BulletManager.h"
#include "Entities/Enemy.h"
#include "Managers/ResourceManager.h"
#include "Map.h"
#include <memory>
#include <vector>

class EnemyManager {
  private:
    ResourceManager *resources;

  public:
    EnemyManager(ResourceManager *resources);

    std::vector<std::unique_ptr<Enemy>> enemies;

    void SpawnZombie(Vector2 pos);

    void Update(Vector2 playerPos, Map *map, BulletManager *bulletManager);

    void Draw();
};
