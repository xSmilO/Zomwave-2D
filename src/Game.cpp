#include "Game.h"
#include "raylib.h"
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
    camera.offset = {screenWidth / 2.0f, screenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    InitWindow(screenWidth, screenHeight, "Zomwave 2D");

    levelMap = new Map();
    player = new Player();

    SpawnPlayer();
}

Game::~Game() {
    delete player;
    delete levelMap;
    CloseWindow();
}

void Game::Draw() {
    // set proper width and height in browser window
#if defined(PLATFORM_WEB)
    int padding = 30; // set padding to avoid scrollbar and browser edge overlap
    SetWindowSize(getBrowserWidth() - padding, getBrowserHeight() - padding);
#endif
    BeginDrawing();
    ClearBackground(RAYWHITE);


    player->Update(levelMap);
    camera.target = player->GetPosition();

    BeginMode2D(camera);

    levelMap->Draw();
    player->Draw();
    // DrawCircleV(mouseWorldPos, 5.0f, RED);

    EndMode2D();

    std::string playerPos = "(" + std::to_string(player->GetPosition().x) +
                            "," + std::to_string(player->GetPosition().y) + ")";
    DrawText("Poruszaj sie WSAD", 10, 10, 20, DARKGRAY);
    DrawText(playerPos.c_str(), 10, 30, 20, DARKGRAY);
    EndDrawing();
}

// method used to call all game logic
void Game::MainLoopHelper(void *userData) {
    Game *game = static_cast<Game *>(userData);

    game->Draw(); // add more game logic methods here eg: Update();
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

void Game::SpawnPlayer() {
    player->SetPosition(levelMap->GetSpawnPoint());
}
