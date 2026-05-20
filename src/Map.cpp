#include "Map.h"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Entity.hpp"
#include "raylib.h"

Map::Map() {
    project.loadFromFile("../assets/level_0.ldtk");
    currentLevel = &project.getWorld().getLevel("Level_0");
    mapSize = {(float)currentLevel->size.x, (float)currentLevel->size.y};

    for (auto &layer : currentLevel->allLayers()) {
        if (layer.hasTileset()) {
            std::string path = layer.getTileset().path;
            if (tilesets.find(path) == tilesets.end()) {
                Texture2D tex = LoadTexture(("../assets/" + path).c_str());
                SetTextureFilter(tex, TEXTURE_FILTER_POINT);
                tilesets[path] = tex;
            }
        }

        if (layer.getName() == "Collisions") {
            collisionLayer = &layer;
            tileSize = layer.getCellSize();
        }
    }
}

Map::~Map() {
    for (auto const &tileset : tilesets) {
        UnloadTexture(tileset.second);
    }
}

Vector2 Map::GetSpawnPoint() {
    for (auto &layer : currentLevel->allLayers()) {
        if (layer.getType() == ldtk::LayerType::Entities) {
            for (auto &entity : layer.allEntities()) {
                if (entity.getName() == "SpawnPoint") {
                    auto pos = entity.getPosition();

                    return {(float)pos.x, (float)pos.y};
                }
            }
        }
    }

    TraceLog(LOG_WARNING, "Brak Entity 'SpawnPoint' w pliku LDtk!");
    return {32.0f, 32.0f};
}

bool Map::CheckHitbox(Rectangle hitbox) {
    if (hitbox.x < 100.0f || hitbox.x + hitbox.width > mapSize.x - 100) {
        return true;
    }

    if (hitbox.y < 100.0f || hitbox.y + hitbox.height > mapSize.y - 100) {
        return true;
    };

    if (collisionLayer == nullptr)
        return false;

    for (auto &tile : collisionLayer->getIntGridValPositions(1)) {
        ldtk::IntPoint pos = tile;

        Rectangle wallRect = {(float)(pos.x * tileSize),
                              (float)(pos.y * tileSize), tileSize, tileSize};

        if (CheckCollisionRecs(hitbox, wallRect))
            return true;
    }

    return false;
}

void Map::DrawLayer(const ldtk::Layer &layer) {
    if (layer.getType() == ldtk::LayerType::Tiles ||
        layer.getType() == ldtk::LayerType::AutoLayer) {
        std::string path = layer.getTileset().path;
        Texture2D currentTexture = tilesets[path];

        // printf("%s\n", path.c_str());

        for (auto &tile : layer.allTiles()) {
            auto src = tile.getTextureRect();

            Rectangle sourceRec = {(float)src.x, (float)src.y, (float)src.width,
                                   (float)src.height};

            if (tile.flipX)
                sourceRec.width *= -1.0f;
            if (tile.flipY)
                sourceRec.height *= -1.0f;

            auto pos = tile.getPosition();
            Rectangle destRec = {(float)pos.x, (float)pos.y, (float)src.width,
                                 (float)src.height};

            DrawTexturePro(currentTexture, sourceRec, destRec, {0, 0}, 0.0f,
                           WHITE);
        }
    }
}

void Map::DrawBackground() {
    auto &layers = currentLevel->allLayers();

    for (int i = layers.size() - 1; i >= 0; --i) {
        if (layers[i].getName() == "PlayerLayer")
            break;

        DrawLayer(layers[i]);
    }
}

void Map::DrawForeground() {
    auto &layers = currentLevel->allLayers();
    bool isForeground = false;

    for (int i = layers.size() - 1; i >= 0; --i) {
        if (layers[i].getName() == "PlayerLayer") {
            isForeground = true;
            continue;
        }

        if (isForeground)
            DrawLayer(layers[i]);
    }

    // for (auto &tile : collisionLayer->getIntGridValPositions(1)) {
    //     ldtk::IntPoint pos = tile;
    //
    //     Rectangle wallRect = {(float)(pos.x * tileSize),
    //                           (float)(pos.y * tileSize), tileSize, tileSize};
    //     DrawRectangleRec(wallRect, GREEN);
    // }
}
