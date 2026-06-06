#pragma once
#include "Managers/ResourceManager.h"
#include "Weapon.h"
#include <map>
#include <raylib.h>

struct SoundPool {
    static const int MAX_ALIASES = 10;
    Sound aliases[MAX_ALIASES];
    int currentIndex = 0;
};

class AudioManager {
  private:
    ResourceManager *resources;

    std::map<WeaponType, SoundPool> weaponPools;
    std::vector<SoundPool> zombieRoarPools;
    // Music *currentMusic = nullptr;

    void RegisterWeaponSound(WeaponType type, Sound baseSound);
    float musicVolume = 0.5f;
    float sfxVolume = 0.5f;
    bool isPlayingMenuMusic = false;
    int currentGameTrackIndex = -1;

    void PlaySpatialSound(Sound sound, Vector2 sourcePos, Vector2 listenerPos);

  public:
    void Init(ResourceManager *resManager);
    void InitializePools();
    void CleanUp();

    void PlayMenuMusic();
    void PlayGameMusic();
    void UpdateMusic();
    void PauseMusic();
    void ResumeMusic();

    void PlayShoot(WeaponType type);
    void PlayReload(WeaponType type);
    void PlayUIClick();
    void PlayBuy();
    void PlayCoinPickup();
    void PlayPotionUse();
    void PlayPlayerStep();

    void PlayZombieAmbient(Vector2 enemyPos, Vector2 playerPos);

    void SetMusicVolume(float volume);
    void SetSFXVolume(float volume);

    float GetMusicVolume() { return musicVolume; }
    float GetSFXVolume() { return sfxVolume; }
};
