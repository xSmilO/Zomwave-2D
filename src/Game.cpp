#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <string>

Game::Game() {
#if defined(PLATFORM_WEB)
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
#else
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT |
                   FLAG_WINDOW_RESIZABLE); // set custom flags for desktop
    SetTargetFPS(
        60); // do not set fps when used in browser for better web performance
#endif

    screenWidth = 1920 / 2;
    screenHeight = 1080 / 2;
    camera.offset = {virtualWidth / 2.0f, virtualHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    InitWindow(screenWidth, screenHeight, "Zomwave 2D");

    target = LoadRenderTexture(virtualWidth, virtualHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    levelMap = new Map();
    player = new Player();
    pistol = new Pistol();
    player->SetWeapon(pistol);

    SpawnPlayer();
}

Game::~Game() {
    delete player;
    delete levelMap;
    delete pistol;
    UnloadRenderTexture(target);
    CloseWindow();
}

void Game::Draw() {
    // set proper width and height in browser window
#if defined(PLATFORM_WEB)
    int padding = 30; // set padding to avoid scrollbar and browser edge overlap
    SetWindowSize(getBrowserWidth() - padding, getBrowserHeight() - padding);
#endif
    BeginTextureMode(target);
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);

    levelMap->DrawBackground();
    player->Draw();
    levelMap->DrawForeground();
    bulletManager.Draw();

    EndMode2D();
    EndTextureMode();

    std::string playerPos = "(" + std::to_string(player->GetPosition().x) +
                            "," + std::to_string(player->GetPosition().y) + ")";

    BeginDrawing();
    ClearBackground(BLACK);

    float scale = fminf((float)GetScreenWidth() / virtualWidth,
                        (float)GetScreenHeight() / virtualHeight);

    Rectangle source = {0.0f, 0.0f, (float)target.texture.width,
                        (float)-target.texture.height};

    Rectangle dest = {(GetScreenWidth() - (virtualWidth * scale)) * 0.5f,
                      (GetScreenHeight() - (virtualHeight * scale)) * 0.5f,
                      virtualWidth * scale, virtualHeight * scale};

    DrawTexturePro(target.texture, source, dest, {0, 0}, 0.0f, WHITE);

    DrawText("Poruszaj sie WSAD", 10, 10, 20, DARKGRAY);
    DrawText(playerPos.c_str(), 10, 30, 20, DARKGRAY);
    EndDrawing();
}

void Game::Update() {
    float scale = fminf((float)GetScreenWidth() / virtualWidth,
                        (float)GetScreenHeight() / virtualHeight);
    bulletManager.Update(levelMap);
    player->Update(mousePosition, levelMap, &bulletManager);
    camera.target = player->GetPosition();

    Vector2 mouseRaw = GetMousePosition();
    mousePosition.x =
        (mouseRaw.x - (GetScreenWidth() - (virtualWidth * scale)) * 0.5f) /
        scale;
    mousePosition.y =
        (mouseRaw.y - (GetScreenHeight() - (virtualHeight * scale)) * 0.5f) /
        scale;

    mousePosition.x = Clamp(mousePosition.x, 0, virtualWidth);
    mousePosition.y = Clamp(mousePosition.y, 0, virtualHeight);

    mousePosition = GetScreenToWorld2D(mousePosition, camera);
}

// method used to call all game logic
void Game::MainLoopHelper(void *userData) {
    Game *game = static_cast<Game *>(userData);
    game->Update();
    game->Draw();
}

// used to run game loop logic
void Game::Run() {
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop_arg(MainLoopHelper, this, 0, 1);
#else
    while (!WindowShouldClose()) {
        MainLoopHelper(this);
    }
#endif
}

void Game::SpawnPlayer() { player->SetPosition(levelMap->GetSpawnPoint()); }
