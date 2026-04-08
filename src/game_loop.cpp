#include "game_loop.h"
#include "raylib.h"

GameLoop::GameLoop() {
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
    player = new Player();

    InitWindow(screenWidth, screenHeight, "Zombie killer");
}

GameLoop::~GameLoop() {
    delete player;
    CloseWindow();
}

void GameLoop::Draw() {
    // set proper width and height in browser window
#if defined(PLATFORM_WEB)
    int padding = 30; // set padding to avoid scrollbar and browser edge overlap
    SetWindowSize(getBrowserWidth() - padding, getBrowserHeight() - padding);
#endif
    BeginDrawing();
    ClearBackground(RAYWHITE);

    mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

    player->Update(mouseWorldPos);
    camera.target = player->GetPosition();

    BeginMode2D(camera);

    levelMap.Draw();
    player->Draw();
    DrawCircleV(mouseWorldPos, 5.0f, RED);

    EndMode2D();

    DrawText("Poruszaj sie WSAD", 10, 10, 20, DARKGRAY);
    EndDrawing();
}

// method used to call all game logic
void GameLoop::MainLoopHelper(void *userData) {
    GameLoop *gameLoop = static_cast<GameLoop *>(userData);

    gameLoop->Draw(); // add more game logic methods here eg: Update();
}

// used to run game loop logic
void GameLoop::Run() {
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop_arg(MainLoopHelper, this, 0, 1);
#else
    while (!WindowShouldClose()) {
        MainLoopHelper(this);
    }
#endif
}
