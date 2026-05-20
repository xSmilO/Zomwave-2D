#pragma once
#include "Entities/Player.h"
#include "Managers/EnemyManager.h"
#include "Managers/ResourceManager.h"
#include "Managers/UIManager.h"
#include "Managers/WaveManager.h"
#include "Map.h"
#include "Weapons/Pistol.h"
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
    ResourceManager *resources;
    UIManager *uiManager;

    BulletManager *bulletManager;
    EnemyManager *enemyManager;
    WaveManager *waveManager;

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
