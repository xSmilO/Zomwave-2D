#pragma once
#include <raylib.h>
#include <vector>

/**
 * @class ResourceManager
 * @brief Loads, stores, and unloads all game assets (textures, sounds, music).
 *
 * All asset loading happens in LoadAll() and unloading in UnloadAll().
 * Textures and sounds are accessed via public member fields.
 */
class ResourceManager {
  public:
    // -- Player textures --
    Texture2D texPlayerIdle; ///< Player idle animation spritesheet.
    Texture2D texPlayerWalk; ///< Player walk animation spritesheet.

    // -- Enemy textures --
    Texture2D texZombie;   ///< Zombie walk animation spritesheet.
    Texture2D texSkeleton; ///< Skeleton walk animation spritesheet.

    // -- Weapon textures --
    Texture2D texGlockShoot;   ///< Glock shooting animation spritesheet.
    Texture2D texGlockReload;  ///< Glock reloading animation spritesheet.
    Texture2D texAk47Shoot;    ///< AK-47 shooting animation spritesheet.
    Texture2D texAk47Reload;   ///< AK-47 reloading animation spritesheet.
    Texture2D texMp5Shoot;     ///< MP5 shooting animation spritesheet.
    Texture2D texMp5Reload;    ///< MP5 reloading animation spritesheet.
    Texture2D texShotgunShoot; ///< Shotgun shooting animation spritesheet.

    // -- Item textures --
    Texture2D texCoin;         ///< Coin pickup texture.
    Texture2D texHealthPotion; ///< Health potion pickup texture.

    // -- Projectile textures --
    Texture2D texBullet; ///< Standard bullet texture.
    Texture2D texArrow;  ///< Arrow projectile texture.

    // -- Equipment textures --
    Texture2D texBow; ///< Bow weapon texture.

    // -- Boss textures --
    Texture2D texBossWalk;   ///< Boss walk animation spritesheet.
    Texture2D texBossAttack; ///< Boss attack animation spritesheet.
    Texture2D texBossIdle;   ///< Boss idle animation spritesheet.

    // -- Environment textures --
    Texture2D texSkyBox; ///< Skybox / background texture.
    Texture2D texBottle; ///< Bottle projectile texture.
    Texture2D texBomb;   ///< Bomb pickup / throwable texture.

    // -- UI sounds --
    Sound sfxUIClick; ///< UI click sound.

    // -- Weapon sounds --
    Sound sfxGlockShoot;    ///< Glock shoot sound.
    Sound sfxGlockReload;   ///< Glock reload sound.
    Sound sfxMP5Shoot;      ///< MP5 shoot sound.
    Sound sfxMP5Reload;     ///< MP5 reload sound.
    Sound sfxAk47Shoot;     ///< AK-47 shoot sound.
    Sound sfxAk47Reload;    ///< AK-47 reload sound.
    Sound sfxShotgunShoot;  ///< Shotgun shoot sound.
    Sound sfxShotgunReload; ///< Shotgun reload sound.

    // -- Gameplay sounds --
    Sound sfxBuy;        ///< Purchase sound.
    Sound sfxCoinPickup; ///< Coin pickup sound.
    Sound sfxPotionUse;  ///< Potion use sound.
    Sound sfxPlayerStep; ///< Player footstep sound.
    Sound sfxBossDrink;  ///< Boss drinking sound.
    Sound sfxBomb;       ///< Bomb explosion sound.
    Sound sfxBowShoot;   ///< Bow shoot sound.

    // -- Ambient / hurt sounds --
    std::vector<Sound> sfxZombieSounds;     ///< Zombie ambient roar sounds.
    std::vector<Sound> sfxBossSounds;       ///< Boss ambient roar sounds.
    std::vector<Sound> sfxPlayerHurtSounds; ///< Player hurt sounds.

    // -- Music --
    Music bgmMenu;                    ///< Menu background music.
    std::vector<Music> bgmGameTracks; ///< Game music tracks (played in sequence).

    /// Loads all textures, sounds, and music from the assets directory.
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

        sfxPlayerHurtSounds.push_back(LoadSound("../assets/audio/player_hurt1.wav"));
        sfxPlayerHurtSounds.push_back(LoadSound("../assets/audio/player_hurt2.wav"));
        sfxPlayerHurtSounds.push_back(LoadSound("../assets/audio/player_hurt3.wav"));

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
        for(size_t i = 0; i < bgmGameTracks.size(); ++i) {
            UnloadMusicStream(bgmGameTracks[i]);
        }

        for(size_t i = 0; i < sfxZombieSounds.size(); ++i) {
            UnloadSound(sfxZombieSounds[i]);
        }

        for(size_t i = 0; i < sfxBossSounds.size(); ++i) {
            UnloadSound(sfxBossSounds[i]);
        }

        for(size_t i = 0; i < sfxPlayerHurtSounds.size(); ++i) {
            UnloadSound(sfxPlayerHurtSounds[i]);
        }
    }
};
