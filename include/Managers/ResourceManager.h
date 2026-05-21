#pragma once
#include <raylib.h>

class ResourceManager {
  public:
    Texture2D texPlayerIdle;
    Texture2D texPlayerWalk;

    Texture2D texZombie;

    Texture2D texPistol;

    Texture2D texCoin;
    Texture2D texHealthPotion;

    void LoadAll() {
        texPlayerIdle = LoadTexture("../assets/player_idle.png");
        texPlayerWalk = LoadTexture("../assets/player_walk.png");

        texPistol = LoadTexture("../assets/pistol.png");

        texZombie = LoadTexture("../assets/zombie_walk.png");

        texCoin = LoadTexture("../assets/coin.png");
        texHealthPotion = LoadTexture("../assets/health_potion.png");
    }

    void UnloadAll() {
        UnloadTexture(texPlayerIdle);
        UnloadTexture(texPlayerWalk);
        UnloadTexture(texPistol);
        UnloadTexture(texZombie);
        UnloadTexture(texCoin);
        UnloadTexture(texHealthPotion);
    }
};
