#pragma once
#include <raylib.h>

class ResourceManager {
  public:
    Texture2D texPlayerIdle;
    Texture2D texPlayerWalk;

    Texture2D texZombie;

    Texture2D texSkeleton;

    Texture2D texPistolShoot;
    Texture2D texPistolReload;

    Texture2D texAk47Shoot;
    Texture2D texAk47Reload;

    Texture2D texMp5Shoot;
    Texture2D texMp5Reload;

    Texture2D texCoin;
    Texture2D texHealthPotion;

    Texture2D texBullet;
    Texture2D texArrow;

    Texture2D texBow;

    void LoadAll() {
        texPlayerIdle = LoadTexture("../assets/player_idle.png");
        texPlayerWalk = LoadTexture("../assets/player_walk.png");

        texPistolShoot = LoadTexture("../assets/glock_shoot.png");
        texPistolReload = LoadTexture("../assets/glock_reload.png");

        texAk47Shoot = LoadTexture("../assets/ak47_shoot.png");
        texAk47Reload = LoadTexture("../assets/ak47_reload.png");

        texMp5Shoot = LoadTexture("../assets/mp5_shoot.png");
        texMp5Reload = LoadTexture("../assets/mp5_reload.png");

        texZombie = LoadTexture("../assets/zombie_walk.png");

        texSkeleton = LoadTexture("../assets/skeleton_walk.png");

        texCoin = LoadTexture("../assets/coin.png");
        texHealthPotion = LoadTexture("../assets/health_potion.png");

        texBullet = LoadTexture("../assets/bullet.png");
        texArrow = LoadTexture("../assets/arrow.png");
        texBow = LoadTexture("../assets/bow.png");
    }

    void UnloadAll() {
        UnloadTexture(texPlayerIdle);
        UnloadTexture(texPlayerWalk);
        UnloadTexture(texPistolShoot);
        UnloadTexture(texPistolReload);
        UnloadTexture(texAk47Shoot);
        UnloadTexture(texAk47Reload);
        UnloadTexture(texMp5Shoot);
        UnloadTexture(texMp5Reload);
        UnloadTexture(texZombie);
        UnloadTexture(texSkeleton);
        UnloadTexture(texCoin);
        UnloadTexture(texHealthPotion);
        UnloadTexture(texBullet);
        UnloadTexture(texArrow);
        UnloadTexture(texBow);
    }
};
