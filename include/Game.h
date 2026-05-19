#pragma once
#include "Managers/EnemyManager.h"
#include "Managers/ResourceManager.h"
#include "Map.h"
#include "Weapons/Pistol.h"
#include "Entities/Player.h"
#include "raylib.h"

class Game {
  private:
    Camera2D camera;
    Vector2 mousePosition;
    Player *player;
    Pistol *pistol;
    int screenWidth{};
    int screenHeight{};
    Map *levelMap;
    ResourceManager* resources;

    BulletManager* bulletManager;
    EnemyManager* enemyManager;
    
    RenderTexture2D target;
    const int virtualWidth = 800;
    const int virtualHeight = 450;

    void Draw();
    void Update();
    void SpawnPlayer();
    static void MainLoopHelper(void *userData);

  public:
    Game();
    ~Game();
    void Run();
};
