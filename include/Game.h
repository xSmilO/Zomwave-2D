#pragma once
#include "Entities/Player.h"
#include "Managers/AudioManager.h"
#include "Managers/EnemyManager.h"
#include "Managers/PickupManager.h"
#include "Managers/ResourceManager.h"
#include "Managers/ShopManager.h"
#include "Managers/UIManager.h"
#include "Managers/WaveManager.h"
#include "Map.h"
#include "raylib.h"

enum class GameState { MAIN_MENU, PLAYING, SETTINGS, PAUSED, EXIT };

class Game {
  private:
    GameState currentState;
    GameState previousState;
    Camera2D camera;
    Vector2 mousePosition;
    Player *player;
    int screenWidth{};
    int screenHeight{};
    Map *levelMap;
    ResourceManager *resources;
    PickupManager *pickupManager;
    UIManager *uiManager;

    BulletManager *bulletManager;
    EnemyManager *enemyManager;
    WaveManager *waveManager;

    ShopManager shopManager;
    AudioManager audioManager;

    std::vector<int> fpsOptions = {30, 60, 90, 120, 144, 180, 240};
    int currentFpsIndex = 1;

    RenderTexture2D target;
    const int virtualWidth = 800;
    const int virtualHeight = 450;

    float masterVolume = 1.0f;

    void Draw();
    void DrawMainMenu();
    void DrawPlaying();
    void DrawSettings();
    void DrawPaused();

    void Update();
    void UpdateMainMenu(float dt);
    void UpdatePlaying(float dt);
    void UpdateSettings(float dt);
    void UpdatePaused(float dt);

    void SpawnPlayer();
    static void MainLoopHelper(void *userData);

  public:
    Game();
    ~Game();
    void Run();
};
