#pragma once
#include <raylib.h>
#include <vector>

class ResourceManager {
  public:
    Texture2D texPlayerIdle;
    Texture2D texPlayerWalk;

    Texture2D texZombie;

    Texture2D texSkeleton;

    Texture2D texGlockShoot;
    Texture2D texGlockReload;

    Texture2D texAk47Shoot;
    Texture2D texAk47Reload;

    Texture2D texMp5Shoot;
    Texture2D texMp5Reload;

    Texture2D texShotgunShoot;

    Texture2D texCoin;
    Texture2D texHealthPotion;

    Texture2D texBullet;
    Texture2D texArrow;

    Texture2D texBow;

    Texture2D texBossWalk;
    Texture2D texBossAttack;
    Texture2D texBossIdle;

    Texture2D texSkyBox;
    Texture2D texBottle;
    Texture2D texBomb;

    Sound sfxUIClick;

    Sound sfxGlockShoot;
    Sound sfxGlockReload;

    Sound sfxMP5Shoot;
    Sound sfxMP5Reload;

    Sound sfxAk47Shoot;
    Sound sfxAk47Reload;

    Sound sfxShotgunShoot;
    Sound sfxShotgunReload;

    Sound sfxBuy;
    Sound sfxCoinPickup;
    Sound sfxPotionUse;
    Sound sfxPlayerStep;
    Sound sfxBossDrink;
    Sound sfxBomb;

    Sound sfxBowShoot;
    std::vector<Sound> sfxZombieSounds;
    std::vector<Sound> sfxBossSounds;
    std::vector<Sound> sfxPlayerHurtSounds;

    Music bgmMenu;
    std::vector<Music> bgmGameTracks;

    void LoadAll() {
        texPlayerIdle = LoadTexture("../assets/player_idle.png");
        texPlayerWalk = LoadTexture("../assets/player_walk.png");

        texGlockShoot = LoadTexture("../assets/glock_shoot.png");
        texGlockReload = LoadTexture("../assets/glock_reload.png");

        texAk47Shoot = LoadTexture("../assets/ak47_shoot.png");
        texAk47Reload = LoadTexture("../assets/ak47_reload.png");

        texMp5Shoot = LoadTexture("../assets/mp5_shoot.png");
        texMp5Reload = LoadTexture("../assets/mp5_reload.png");

        texShotgunShoot = LoadTexture("../assets/shotgun_shoot.png");

        texZombie = LoadTexture("../assets/zombie_walk.png");
        texSkeleton = LoadTexture("../assets/skeleton_walk.png");
        texBossWalk = LoadTexture("../assets/boss_walk.png");
        texBossAttack = LoadTexture("../assets/boss_attack.png");
        texBossIdle = LoadTexture("../assets/boss_idle.png");

        texCoin = LoadTexture("../assets/coin.png");
        texHealthPotion = LoadTexture("../assets/health_potion.png");

        texBullet = LoadTexture("../assets/bullet.png");
        texArrow = LoadTexture("../assets/arrow.png");
        texBow = LoadTexture("../assets/bow.png");
        texSkyBox = LoadTexture("../assets/skybox.png");
        texBottle = LoadTexture("../assets/bottle.png");
        texBomb = LoadTexture("../assets/bomb.png");

        sfxUIClick = LoadSound("../assets/audio/ui_click.mp3");

        sfxGlockShoot = LoadSound("../assets/audio/glock_2.mp3");
        sfxGlockReload = LoadSound("../assets/audio/glock_reload.wav");

        sfxMP5Shoot = LoadSound("../assets/audio/smg_shoot.wav");
        sfxMP5Reload = LoadSound("../assets/audio/mp5_reload.wav");

        sfxAk47Shoot = LoadSound("../assets/audio/ak47_shoot.wav");
        sfxAk47Reload = LoadSound("../assets/audio/ak47_reload.wav");

        sfxBuy = LoadSound("../assets/audio/buy.mp3");
        sfxCoinPickup = LoadSound("../assets/audio/coin_pickup.mp3");
        sfxPotionUse = LoadSound("../assets/audio/potion_use.wav");
        sfxPlayerStep = LoadSound("../assets/audio/player_step.wav");
        sfxBossDrink = LoadSound("../assets/audio/drinking.wav");
        sfxBomb = LoadSound("../assets/audio/bomb.wav");
        sfxShotgunShoot = LoadSound("../assets/audio/shotgun_shoot.wav");
        sfxShotgunReload = LoadSound("../assets/audio/shotgun_reload.wav");

        sfxBowShoot = LoadSound("../assets/audio/bow_shoot.mp3");
        sfxZombieSounds.push_back(LoadSound("../assets/audio/zombie_1.mp3"));
        sfxZombieSounds.push_back(LoadSound("../assets/audio/zombie_2.mp3"));
        sfxZombieSounds.push_back(LoadSound("../assets/audio/zombie_3.mp3"));
        sfxZombieSounds.push_back(LoadSound("../assets/audio/zombie_4.mp3"));

        sfxBossSounds.push_back(LoadSound("../assets/audio/boss_1.wav"));
        sfxBossSounds.push_back(LoadSound("../assets/audio/boss_2.wav"));
        sfxBossSounds.push_back(LoadSound("../assets/audio/boss_3.wav"));
        sfxBossSounds.push_back(LoadSound("../assets/audio/boss_4.wav"));
        sfxBossSounds.push_back(LoadSound("../assets/audio/boss_5.wav"));

        sfxPlayerHurtSounds.push_back(
            LoadSound("../assets/audio/player_hurt1.wav"));
        sfxPlayerHurtSounds.push_back(
            LoadSound("../assets/audio/player_hurt2.wav"));
        sfxPlayerHurtSounds.push_back(
            LoadSound("../assets/audio/player_hurt3.wav"));

        bgmMenu = LoadMusicStream("../assets/music/main_menu_music.mp3");
        bgmMenu.looping = true;

        Music track1 = LoadMusicStream("../assets/music/game_track1.mp3");
        track1.looping = false;
        bgmGameTracks.push_back(track1);

        Music track2 = LoadMusicStream("../assets/music/game_track2.mp3");
        track2.looping = false;
        bgmGameTracks.push_back(track2);

        Music track3 = LoadMusicStream("../assets/music/game_track3.mp3");
        track1.looping = false;
        bgmGameTracks.push_back(track3);

        Music track4 = LoadMusicStream("../assets/music/game_track4.mp3");
        track4.looping = false;
        bgmGameTracks.push_back(track4);

        Music track5 = LoadMusicStream("../assets/music/game_track5.mp3");
        track5.looping = false;
        bgmGameTracks.push_back(track5);

        Music track6 = LoadMusicStream("../assets/music/game_track6.mp3");
        track6.looping = false;
        bgmGameTracks.push_back(track6);
    }

    void UnloadAll() {
        UnloadTexture(texPlayerIdle);
        UnloadTexture(texPlayerWalk);
        UnloadTexture(texGlockShoot);
        UnloadTexture(texGlockReload);
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
        UnloadTexture(texSkyBox);
        UnloadTexture(texBossWalk);
        UnloadTexture(texBossAttack);
        UnloadTexture(texBossIdle);
        UnloadTexture(texBottle);
        UnloadTexture(texShotgunShoot);
        UnloadTexture(texBomb);

        UnloadSound(sfxUIClick);
        UnloadSound(sfxGlockShoot);
        UnloadSound(sfxGlockReload);
        UnloadSound(sfxMP5Shoot);
        UnloadSound(sfxMP5Reload);
        UnloadSound(sfxAk47Shoot);
        UnloadSound(sfxAk47Reload);
        UnloadSound(sfxBuy);
        UnloadSound(sfxCoinPickup);
        UnloadSound(sfxPotionUse);
        UnloadSound(sfxPlayerStep);
        UnloadSound(sfxShotgunShoot);
        UnloadSound(sfxShotgunReload);
        UnloadSound(sfxBowShoot);
        UnloadSound(sfxBomb);

        UnloadMusicStream(bgmMenu);
        for (size_t i = 0; i < bgmGameTracks.size(); ++i) {
            UnloadMusicStream(bgmGameTracks[i]);
        }

        for (size_t i = 0; i < sfxZombieSounds.size(); ++i) {
            UnloadSound(sfxZombieSounds[i]);
        }

        for (size_t i = 0; i < sfxBossSounds.size(); ++i) {
            UnloadSound(sfxBossSounds[i]);
        }

        for (size_t i = 0; i < sfxPlayerHurtSounds.size(); ++i) {
            UnloadSound(sfxPlayerHurtSounds[i]);
        }
    }
};
