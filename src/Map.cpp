#include "Map.h"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Entity.hpp"
#include "raylib.h"

Map::Map() { LoadMap(); }

Map::~Map() { UnloadTexture(layerTex); }

void Map::LoadMap() {
    tiles.clear();

    try {
        proj.loadFromFile("../assets/basic_map.ldtk");
        const ldtk::World &world = proj.getWorld();

        const ldtk::Tileset &tileset = world.getTileset("Demo");

        layerTex = LoadTexture(("../assets/" + tileset.path).c_str());

        const ldtk::Level &level = world.getLevel("Level_0");
        const ldtk::Layer &layer = level.getLayer("Tiles");
        collisionLayer = &level.getLayer("Collisions");
        const ldtk::Entity &spawnEntity =
            level.getLayer("SpawnPoints").getEntitiesByName("PlayerSpawn")[0];

        spawnPoint = {(float)spawnEntity.getWorldPosition().x,
                      (float)spawnEntity.getWorldPosition().y};

        tileSize = tileset.tile_size;
        mapWidth = layer.getGridSize().x;
        mapHeight = layer.getGridSize().y;

        for (const ldtk::Tile &tile : layer.allTiles()) {
            tiles.push_back(tile);
            tile.getGridPosition();
        }

    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

void Map::Draw() {
    for (int i = 0; i < tiles.size(); ++i) {
        ldtk::Tile tile = tiles[i];
        ldtk::IntRect texPos = tile.getTextureRect();

        Rectangle sourceRec = {(float)texPos.x, (float)texPos.y,
                               (float)tileSize, (float)tileSize};

        ldtk::IntPoint tilePos = tiles[i].getPosition();
        Vector2 destPos = {(float)tilePos.x, (float)tilePos.y};

        DrawTextureRec(layerTex, sourceRec, destPos, WHITE);
    }
}

Vector2 Map::GetSpawnPoint() { return spawnPoint; }

bool Map::CheckHitbox(Rectangle hitbox) {
    if (collisionLayer == nullptr)
        return false;
    float left = hitbox.x;
    float right = hitbox.x + hitbox.width - 1.0f;
    float top = hitbox.y;
    float bottom = hitbox.y + hitbox.height - 1.0f;

    int minGridX = (int)(left / tileSize);
    int maxGridX = (int)(right / tileSize);
    int minGridY = (int)(top / tileSize);
    int maxGridY = (int)(bottom / tileSize);

    if (minGridX < 0 || minGridY < 0 || maxGridX >= mapWidth ||
        maxGridY >= mapHeight)
        return true;

    for (int y = minGridY; y <= maxGridY; ++y) {
        for (int x = minGridX; x <= maxGridX; ++x) {
            if (collisionLayer->getIntGridVal(x, y).value == 1)
                return true;
        }
    }

    return false;
}
