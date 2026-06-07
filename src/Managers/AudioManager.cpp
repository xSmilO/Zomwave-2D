#include "Managers/AudioManager.h"
#include "raylib.h"
#include "raymath.h"

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
    RegisterWeaponSound(WeaponType::SHOTGUN, resources->sfxShotgunShoot);
    RegisterWeaponSound(WeaponType::BOW, resources->sfxBowShoot);

    InitializePools();
}

void AudioManager::InitializePools() {
    for (Sound baseSound : resources->sfxZombieSounds) {
        SoundPool pool;
        pool.currentIndex = 0;

        for (int i = 0; i < SoundPool::MAX_ALIASES; i++) {
            pool.aliases[i] = LoadSoundAlias(baseSound);
        }

        zombieRoarPools.push_back(pool);
    }

    for (Sound baseSound : resources->sfxBossSounds) {
        SoundPool pool;
        pool.currentIndex = 0;

        for (int i = 0; i < SoundPool::MAX_ALIASES; i++) {
            pool.aliases[i] = LoadSoundAlias(baseSound);
        }

        bossRoarPools.push_back(pool);
    }

    bossDrink.currentIndex = 0;
    for (int i = 0; i < SoundPool::MAX_ALIASES; i++) {
        bossDrink.aliases[i] = LoadSoundAlias(resources->sfxBossDrink);
    }

    bombExplosion.currentIndex = 0;
    for (int i = 0; i < SoundPool::MAX_ALIASES; i++) {
        bombExplosion.aliases[i] = LoadSoundAlias(resources->sfxBomb);
    }
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
    case WeaponType::SHOTGUN:
        soundToPlay = resources->sfxShotgunReload;
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

void AudioManager::PlaySpatialSound(Sound sound, Vector2 sourcePos,
                                    Vector2 listenerPos) {
    float maxHearDistance = 800.0f;

    float dist = Vector2Distance(sourcePos, listenerPos);

    if (dist > maxHearDistance)
        return;

    float volumeFactor = 1.0f - (dist / maxHearDistance);
    float finalVolume = volumeFactor * sfxVolume;

    float dx = sourcePos.x - listenerPos.x;

    float pan = 0.5 + (dx / maxHearDistance) * 0.5f;

    if (pan < 0.0f)
        pan = 0.0f;
    if (pan > 1.0f)
        pan = 1.0f;

    SetSoundVolume(sound, finalVolume);
    SetSoundPan(sound, pan);
    SetSoundPitch(sound, GetRandomValue(90, 110) / 100.0f);

    PlaySound(sound);
}

void AudioManager::PlayPlayerStep() {
    SetSoundVolume(resources->sfxPlayerStep, sfxVolume);
    SetSoundPitch(resources->sfxPlayerStep, GetRandomValue(90, 110) / 100.0f);
    PlaySound(resources->sfxPlayerStep);
}

void AudioManager::PlayPlayerHurt() {
    if (resources->sfxPlayerHurtSounds.empty())
        return;

    int randomSoundIndex =
        GetRandomValue(0, resources->sfxPlayerHurtSounds.size() - 1);

    Sound &soundToPlay = resources->sfxPlayerHurtSounds[randomSoundIndex];

    SetSoundVolume(soundToPlay, sfxVolume);
    PlaySound(soundToPlay);
}

void AudioManager::PlayZombieAmbient(Vector2 enemyPos, Vector2 playerPos) {
    if (zombieRoarPools.empty())
        return;

    int randomSoundIndex = GetRandomValue(0, zombieRoarPools.size() - 1);
    SoundPool &selectedPool = zombieRoarPools[randomSoundIndex];

    Sound &roarToPlay = selectedPool.aliases[selectedPool.currentIndex];

    selectedPool.currentIndex =
        (selectedPool.currentIndex + 1) % SoundPool::MAX_ALIASES;

    PlaySpatialSound(roarToPlay, enemyPos, playerPos);
}

void AudioManager::PlayBossAmbient(Vector2 enemyPos, Vector2 playerPos) {
    if (bossRoarPools.empty())
        return;

    int randomSoundIndex = GetRandomValue(0, bossRoarPools.size() - 1);
    SoundPool &selectedPool = bossRoarPools[randomSoundIndex];

    Sound &roarToPlay = selectedPool.aliases[selectedPool.currentIndex];

    selectedPool.currentIndex =
        (selectedPool.currentIndex + 1) % SoundPool::MAX_ALIASES;

    PlaySpatialSound(roarToPlay, enemyPos, playerPos);
}

void AudioManager::PlayBossDrink(Vector2 enemyPos, Vector2 playerPos) {
    PlaySpatialSound(bossDrink.aliases[bossDrink.currentIndex], enemyPos,
                     playerPos);
    bossDrink.currentIndex =
        (bossDrink.currentIndex + 1) % SoundPool::MAX_ALIASES;
}

void AudioManager::PlayBombExplosion(Vector2 bombPos, Vector2 playerPos) {
    PlaySpatialSound(bombExplosion.aliases[bombExplosion.currentIndex], bombPos,
                     playerPos);
    bombExplosion.currentIndex =
        (bombExplosion.currentIndex + 1) % SoundPool::MAX_ALIASES;
}
