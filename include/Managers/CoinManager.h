#pragma once

#include "Entities/Player.h"
#include "raylib.h"
#include <vector>

/**
 * @struct Coin
 * @brief Represents a coin pickup lying on the ground.
 */
struct Coin {
    Vector2 position;   ///< World position of the coin.
    int value;          ///< Coin value (amount added to player's coins).
    bool active = true; ///< Whether the coin is still available for pickup.
};

/**
 * @class CoinManager
 * @brief Manages coin pickups: spawning, magnet attraction, and collection.
 */
class CoinManager {
  private:
    float magnetRadius = 50.0f; ///< Distance at which coins start magnetising toward the player.
    float magnetSpeed = 400.0f; ///< Speed at which coins move toward the player when magnetised.
    float pickupRadius = 25.0f; ///< Distance at which coins are automatically picked up.

  public:
    std::vector<Coin> coins; ///< List of active coin pickups.

    /**
     * @brief Spawns a coin pickup at the given position.
     * @param pos   World position to spawn the coin.
     * @param value Coin value.
     */
    void SpawnCoin(Vector2 pos, int value);

    /**
     * @brief Updates all coins (magnet attraction, pickup detection).
     * @param dt            Delta time in seconds.
     * @param player        Pointer to the player (for position and collection).
     * @param audioManager  Pointer to the audio manager (for pickup sounds).
     */
    void Update(float dt, Player *player, AudioManager *audioManager);

    /**
     * @brief Draws all active coins.
     * @param coinTexture The coin texture to render.
     */
    void Draw(Texture2D coinTexture);

    /// Removes all coins from the scene.
    void Clear();
};
