#pragma once
#include "Map.h"
#include "Pistol.h"
#include "Player.h"
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
    
    RenderTexture2D target;
    const int virtualWidth = 800;
    const int virtualHeight = 600;

    void Draw();
    void Update();
    void SpawnPlayer();
    static void MainLoopHelper(void *userData);

  public:
    Game();
    ~Game();
    void Run();
};
