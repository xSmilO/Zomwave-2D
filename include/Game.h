#pragma once
#include "Entities/Player.h"
#include "Managers/AudioManager.h"
#include "Managers/BombManager.h"
#include "Managers/CoinManager.h"
#include "Managers/EnemyManager.h"
#include "Managers/GameBalance.h"
#include "Managers/ResourceManager.h"
#include "Managers/ShopManager.h"
#include "Managers/UIManager.h"
#include "Managers/WaveManager.h"
#include "Map.h"
#include "raylib.h"
#include <nlohmann/json.hpp>

/**
 * @enum GameState
 * @brief Possible states of the main game loop.
 */
enum class GameState {
    MAIN_MENU,
    PLAYING,
    SETTINGS,
    PAUSED,
    EXIT,
    GAME_OVER
};

/**
 * @struct SaveData
 * @brief Persistent data saved between sessions.
 */
struct SaveData {
    int highScoreWave = 0; ///< Highest wave number the player has reached.
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SaveData, highScoreWave)

/**
 * @class Game
 * @brief Central game class that owns all subsystems and runs the main loop.
 *
 * Manages game state transitions (menu, playing, paused, settings, game over),
 * coordinates all managers, and handles rendering via a virtual-resolution
 * render target with a light-mask overlay.
 */
class Game {
  private:
    SaveData saveData;          ///< Persistent save data (high score).
    GameBalance gameBalance;    ///< Loaded game balance configuration.
    GameState currentState;     ///< Current state of the game loop.
    GameState previousState;    ///< Previous state (used when unpausing).
    Camera2D camera;            ///< World-space camera following the player.
    Vector2 mousePosition;      ///< Current mouse position in world coordinates.
    Player *player;             ///< The player entity.
    int screenWidth{};          ///< Current window width in pixels.
    int screenHeight{};         ///< Current window height in pixels.
    Map *levelMap;              ///< The game map.
    ResourceManager *resources; ///< Resource manager (textures, sounds, music).
    UIManager *uiManager;       ///< UI overlay manager.

    BulletManager *bulletManager; ///< Manages all bullet projectiles.
    EnemyManager *enemyManager;   ///< Manages all enemy entities.
    WaveManager *waveManager;     ///< Manages wave progression and spawning.

    CoinManager coinManager;   ///< Manages coin pickups.
    ShopManager shopManager;   ///< Manages the in-game shop UI.
    AudioManager audioManager; ///< Manages audio playback.
    BombManager bombManager;   ///< Manages thrown bombs.

    std::vector<int> fpsOptions = {30, 60, 90, 120, 144, 180, 240}; ///< Available FPS limits.
    int currentFpsIndex = 1; ///< Index into fpsOptions for the current FPS limit.

    RenderTexture2D lightMask;     ///< Render target for the dynamic light mask.
    RenderTexture2D target;        ///< Virtual-resolution render target (800x450).
    const int virtualWidth = 800;  ///< Virtual resolution width.
    const int virtualHeight = 450; ///< Virtual resolution height.

    float masterVolume = 1.0f; ///< Master volume multiplier.

    void Draw();
    void DrawMainMenu();
    void DrawPlaying();
    void DrawSettings();
    void DrawPaused();
    void DrawGameOver();
    void DrawLights();

    void Update();
    void UpdateMainMenu(float dt);
    void UpdatePlaying(float dt);
    void UpdateSettings(float dt);
    void UpdatePaused(float dt);

    void SpawnPlayer();
    static void MainLoopHelper(void *userData);

    void LoadSave();
    void SaveGame();

  public:
    Game();
    ~Game();

    /// Starts and runs the main game loop until EXIT state is reached.
    void Run();
};
