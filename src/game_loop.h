#pragma once
#include "player.h"
#include "map.h"

class GameLoop {
  private:
    Vector2 mouseWorldPos;
    Camera2D camera = {0};
    Player *player;
    int screenWidth{};
    int screenHeight{};
    GameMap levelMap;


    void Draw();
    static void MainLoopHelper(void *userData);

  public:
    GameLoop();
    ~GameLoop();
    void Run();
};
