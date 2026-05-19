#pragma once
#include <raylib.h>

class ResourceManager {
  public:
    Texture2D playerIdle;
    Texture2D playerWalk;

    Texture2D zombieTex;

    Texture2D pistolTex;

    void LoadAll() {
        playerIdle = LoadTexture("../assets/player_idle.png");
        playerWalk = LoadTexture("../assets/player_walk.png");

        pistolTex = LoadTexture("../assets/pistol.png");

        zombieTex = LoadTexture("../assets/zombie_walk.png");
        // playerIdle = LoadTexture("../assets/player-idle.png");
        // playerIdle = LoadTexture("../assets/player-idle.png");
    }

    void UnloadAll() {
        UnloadTexture(playerIdle);
        UnloadTexture(playerWalk);
        UnloadTexture(pistolTex);
        UnloadTexture(zombieTex);
    }
};
