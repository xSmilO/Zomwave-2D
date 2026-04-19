#pragma once
#include "Map.h"
#include "Player.h"

class Game {
  private:
    Camera2D camera;
    Player *player;
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
