#include "Managers/AudioManager.h"
#include "raylib.h"

void AudioManager::RegisterWeaponSound(WeaponType type, Sound baseSound) {
    SoundPool pool;
    pool.currentIndex = 0;

    for (int i = 0; i < SoundPool::MAX_ALIASES; i++) {
        pool.aliases[i] = LoadSoundAlias(baseSound);
    }

    weaponPools[type] = pool;
}

void AudioManager::Init(ResourceManager *resManager) {
    resources = resManager;

    RegisterWeaponSound(WeaponType::GLOCK, resources->sfxGlockShoot);
    RegisterWeaponSound(WeaponType::MP5, resources->sfxMP5Shoot);
    RegisterWeaponSound(WeaponType::AK47, resources->sfxAk47Shoot);
}

void AudioManager::CleanUp() {
    for (auto &pair : weaponPools) {
        for (int i = 0; i < SoundPool::MAX_ALIASES; i++) {
            UnloadSoundAlias(pair.second.aliases[i]);
        }
    }
}

void AudioManager::PlayShoot(WeaponType type) {
    if (weaponPools.find(type) == weaponPools.end())
        return;

    SoundPool &pool = weaponPools[type];

    SetSoundVolume(pool.aliases[pool.currentIndex], sfxVolume);
    SetSoundPitch(pool.aliases[pool.currentIndex],
                  GetRandomValue(95, 105) / 100.0f);
    PlaySound(pool.aliases[pool.currentIndex]);

    pool.currentIndex = (pool.currentIndex + 1) % SoundPool::MAX_ALIASES;
}

void AudioManager::PlayReload(WeaponType type) {
    Sound soundToPlay;

    switch (type) {
    case WeaponType::GLOCK:
        soundToPlay = resources->sfxGlockReload;
        break;
    case WeaponType::MP5:
        soundToPlay = resources->sfxMP5Reload;
        break;
    case WeaponType::AK47:
        soundToPlay = resources->sfxAk47Reload;
        break;
    default:
        return;
    }

    SetSoundVolume(soundToPlay, sfxVolume);
    PlaySound(soundToPlay);
}

void AudioManager::PlayUIClick() {
    SetSoundVolume(resources->sfxUIClick, sfxVolume);
    PlaySound(resources->sfxUIClick);
}

void AudioManager::PlayBuy() {
    SetSoundVolume(resources->sfxBuy, sfxVolume);
    PlaySound(resources->sfxBuy);
}

void AudioManager::PlayCoinPickup() {
    SetSoundPitch(resources->sfxCoinPickup, GetRandomValue(95, 105) / 100.0f);
    SetSoundVolume(resources->sfxCoinPickup, sfxVolume);
    PlaySound(resources->sfxCoinPickup);
}

void AudioManager::PlayPotionUse() {
    SetSoundVolume(resources->sfxPotionUse, sfxVolume);
    PlaySound(resources->sfxPotionUse);
}

void AudioManager::UpdateMusic() {
    if (resources == nullptr)
        return;

    if (isPlayingMenuMusic) {
        UpdateMusicStream(resources->bgmMenu);
    } else if (currentGameTrackIndex >= 0) {
        UpdateMusicStream(resources->bgmGameTracks[currentGameTrackIndex]);

        Music &currentTrack = resources->bgmGameTracks[currentGameTrackIndex];
        if (!currentTrack.looping) {
            float timePlayed = GetMusicTimePlayed(currentTrack);
            float timeLength = GetMusicTimeLength(currentTrack);
            if (timePlayed >= timeLength - 0.1f) {
                PlayGameMusic();
            }
        }
    }
}

void AudioManager::PlayMenuMusic() {
    if (currentGameTrackIndex >= 0) {
        StopMusicStream(resources->bgmGameTracks[currentGameTrackIndex]);
        currentGameTrackIndex = -1;
    }

    isPlayingMenuMusic = true;
    printf("gra menu %f\n", musicVolume);
    PlayMusicStream(resources->bgmMenu);
    ::SetMusicVolume(resources->bgmMenu, musicVolume);
}

void AudioManager::PlayGameMusic() {
    if (isPlayingMenuMusic) {
        StopMusicStream(resources->bgmMenu);
        isPlayingMenuMusic = false;
    }

    if (resources->bgmGameTracks.empty())
        return;

    currentGameTrackIndex =
        GetRandomValue(0, resources->bgmGameTracks.size() - 1);

    PlayMusicStream(resources->bgmGameTracks[currentGameTrackIndex]);
    ::SetMusicVolume(resources->bgmGameTracks[currentGameTrackIndex],
                     musicVolume);
}

void AudioManager::PauseMusic() {
    if (currentGameTrackIndex >= 0)
        PauseMusicStream(resources->bgmGameTracks[currentGameTrackIndex]);
}

void AudioManager::ResumeMusic() {
    if (currentGameTrackIndex >= 0)
        ResumeMusicStream(resources->bgmGameTracks[currentGameTrackIndex]);
}

void AudioManager::SetMusicVolume(float volume) {
    musicVolume = volume;

    printf("Ustawiam %f\n", musicVolume);

    if (resources == nullptr)
        return;

    if (isPlayingMenuMusic)
        ::SetMusicVolume(resources->bgmMenu, musicVolume);
    else if (currentGameTrackIndex >= 0) {
        ::SetMusicVolume(resources->bgmGameTracks[currentGameTrackIndex],
                         musicVolume);
    }
}

void AudioManager::SetSFXVolume(float volume) { sfxVolume = volume; }
