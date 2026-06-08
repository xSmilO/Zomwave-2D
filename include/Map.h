#pragma once

#include "raylib.h"
#include <LDtkLoader/Project.hpp>
#include <map>

/**
 * @class Map
 * @brief Handles loading, rendering, and collision detection for the game world.
 *
 * Uses the LDtk map format to load tile layers. Provides background/foreground
 * rendering, collision queries, and random spawn point selection.
 */
class Map {
  private:
    ldtk::Project project;                       ///< The loaded LDtk project.
    const ldtk::Level *currentLevel;             ///< Pointer to the currently active level.
    const ldtk::Layer *collisionLayer = nullptr; ///< Pointer to the collision tile layer.
    float tileSize = 16.0f;                      ///< Size of a single tile in world units.
    Vector2 mapSize;                             ///< Total size of the map in world units.

    std::map<std::string, Texture2D> tilesets; ///< Cached tileset textures keyed by name.

    /**
     * @brief Draws a single LDtk layer (tiles).
     * @param layer Reference to the layer to render.
     */
    void DrawLayer(const ldtk::Layer &layer);

  public:
    std::vector<Vector2> validSpawnPoints = {}; ///< Pre-computed list of valid enemy spawn positions.

    Map();
    ~Map();

    /**
     * @brief Returns a random valid spawn point from the pre-computed list.
     * @return A Vector2 position suitable for spawning an entity.
     */
    Vector2 GetSpawnPoint();

    /**
     * @brief Checks whether a given rectangle collides with the collision layer.
     * @param hitbox The rectangle to test (in world coordinates).
     * @return True if the rectangle overlaps a collision tile.
     */
    bool CheckHitbox(Rectangle hitbox);

    /// Draws the background tile layers.
    void DrawBackground();

    /// Draws the foreground tile layers.
    void DrawForeground();
};
