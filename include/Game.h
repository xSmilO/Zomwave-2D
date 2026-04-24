#pragma once
#include "Map.h"
#include "Pistol.h"
#include "Player.h"

class Game {
  private:
    Camera2D camera;
    Vector2 mousePosition;
    Player *player;
    Pistol *pistol;
    int screenWidth{};
    int screenHeight{};
    Map *levelMap;

    void Draw();
    void SpawnPlayer();
    static void MainLoopHelper(void *userData);

  public:
    Game();
    ~Game();
    void Run();
};
