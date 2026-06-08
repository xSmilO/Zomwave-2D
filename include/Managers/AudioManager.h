#pragma once
#include "Managers/ResourceManager.h"
#include "Weapon.h"
#include <map>
#include <raylib.h>

/**
 * @struct SoundPool
 * @brief A circular buffer of sound aliases for playing the same sound
 *        multiple times simultaneously without cutting off previous instances.
 */
struct SoundPool {
    static const int MAX_ALIASES = 10; ///< Maximum number of simultaneous instances.
    Sound aliases[MAX_ALIASES];        ///< Array of sound aliases.
    int currentIndex = 0;              ///< Index of the next alias to use (round-robin).
};

/**
 * @class AudioManager
 * @brief Manages all audio playback: music, SFX, and spatial sounds.
 *
 * Provides methods for playing weapon sounds, enemy ambients, UI clicks,
 * music tracks, and spatial audio that attenuates based on distance
 * between the source and the listener (player).
 */
class AudioManager {
  private:
    ResourceManager *resources; ///< Pointer to the resource manager for sound/music assets.

    std::map<WeaponType, SoundPool> weaponPools; ///< Sound pools per weapon type.
    std::vector<SoundPool> zombieRoarPools;      ///< Sound pools for zombie ambient roars.
    std::vector<SoundPool> bossRoarPools;        ///< Sound pools for boss ambient roars.
    SoundPool bossDrink;                         ///< Sound pool for the boss drinking sound.
    SoundPool bombExplosion;                     ///< Sound pool for bomb explosion sounds.
    // Music *currentMusic = nullptr;

    /**
     * @brief Registers a base sound into a weapon's sound pool (creates aliases).
     * @param type      Weapon type to register the sound for.
     * @param baseSound The base sound to create aliases from.
     */
    void RegisterWeaponSound(WeaponType type, Sound baseSound);

    float musicVolume = 0.5f;        ///< Music volume (0.0 - 1.0).
    float sfxVolume = 0.5f;          ///< SFX volume (0.0 - 1.0).
    bool isPlayingMenuMusic = false; ///< Whether menu music is currently playing.
    int currentGameTrackIndex = -1;  ///< Index of the currently playing game music track.

    /**
     * @brief Plays a sound with spatial attenuation based on source-listener distance.
     * @param sound      The sound to play.
     * @param sourcePos  World position of the sound source.
     * @param listenerPos World position of the listener (player).
     */
    void PlaySpatialSound(Sound sound, Vector2 sourcePos, Vector2 listenerPos);

  public:
    /**
     * @brief Initialises the audio manager with a resource manager.
     * @param resManager Pointer to the resource manager.
     */
    void Init(ResourceManager *resManager);

    /// Creates sound pools for all weapon types and enemy sounds.
    void InitializePools();

    /// Unloads all sound aliases and cleans up.
    void CleanUp();

    /// Starts playing the menu background music.
    void PlayMenuMusic();

    /// Starts or switches to game music tracks.
    void PlayGameMusic();

    /// Updates music stream buffers (must be called every frame while music plays).
    void UpdateMusic();

    /// Pauses the currently playing music.
    void PauseMusic();

    /// Resumes the paused music.
    void ResumeMusic();

    /**
     * @brief Plays the shoot sound for a given weapon type.
     * @param type The weapon type whose shoot sound to play.
     */
    void PlayShoot(WeaponType type);

    /**
     * @brief Plays the reload sound for a given weapon type.
     * @param type The weapon type whose reload sound to play.
     */
    void PlayReload(WeaponType type);

    /// Plays the UI click sound.
    void PlayUIClick();

    /// Plays the buy/purchase sound.
    void PlayBuy();

    /// Plays the coin pickup sound.
    void PlayCoinPickup();

    /// Plays the potion use sound.
    void PlayPotionUse();

    /// Plays a random footstep sound.
    void PlayPlayerStep();

    /// Plays a random player hurt sound.
    void PlayPlayerHurt();

    /**
     * @brief Plays a zombie ambient roar with spatial attenuation.
     * @param enemyPos  World position of the zombie.
     * @param playerPos World position of the player.
     */
    void PlayZombieAmbient(Vector2 enemyPos, Vector2 playerPos);

    /**
     * @brief Plays a boss ambient roar with spatial attenuation.
     * @param enemyPos  World position of the boss.
     * @param playerPos World position of the player.
     */
    void PlayBossAmbient(Vector2 enemyPos, Vector2 playerPos);

    /**
     * @brief Plays the boss drinking sound with spatial attenuation.
     * @param enemyPos  World position of the boss.
     * @param playerPos World position of the player.
     */
    void PlayBossDrink(Vector2 enemyPos, Vector2 playerPos);

    /**
     * @brief Plays a bomb explosion sound with spatial attenuation.
     * @param bombPos   World position of the bomb.
     * @param playerPos World position of the player.
     */
    void PlayBombExplosion(Vector2 bombPos, Vector2 playerPos);

    /**
     * @brief Sets the music volume.
     * @param volume Volume level (0.0 - 1.0).
     */
    void SetMusicVolume(float volume);

    /**
     * @brief Sets the SFX volume.
     * @param volume Volume level (0.0 - 1.0).
     */
    void SetSFXVolume(float volume);

    /// @return Current music volume (0.0 - 1.0).
    float GetMusicVolume() {
        return musicVolume;
    }

    /// @return Current SFX volume (0.0 - 1.0).
    float GetSFXVolume() {
        return sfxVolume;
    }
};
