#include "Game.h"
#include "Managers/BulletManager.h"
#include "Managers/EnemyManager.h"
#include "Managers/UIManager.h"
#include "Managers/WaveManager.h"
#include "raylib.h"
#include "raymath.h"
#include <cmath>

Game::Game() {
#if defined(PLATFORM_WEB)
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
#else
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT |
                   FLAG_WINDOW_RESIZABLE); // set custom flags for desktop
    SetTargetFPS(
        144); // do not set fps when used in browser for better web performance
#endif

    screenWidth = 1920 / 2;
    screenHeight = 1080 / 2;
    camera.offset = {virtualWidth / 2.0f, virtualHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    InitWindow(screenWidth, screenHeight, "Zomwave 2D");

    resources = new ResourceManager();
    resources->LoadAll();

    pickupManager =
        new PickupManager(&resources->texHealthPotion, &resources->texCoin);

    uiManager = new UIManager(&resources->texHealthPotion, &resources->texCoin);

    bulletManager = new BulletManager(&resources->texBullet, &resources->texArrow);
    enemyManager = new EnemyManager(resources, bulletManager);
    waveManager = new WaveManager();

    target = LoadRenderTexture(virtualWidth, virtualHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    levelMap = new Map();
    player = new Player(resources);

    SpawnPlayer();
    // enemyManager.SpawnEnemy({200.0f, 200.0f});
    // enemyManager.SpawnEnemy({400.0f, 150.0f});
    // enemyManager->SpawnZombie({704.0f, 600.0f});
    enemyManager->SpawnSkeleton({500.0f, 350.0f});
}

Game::~Game() {
    resources->UnloadAll();
    delete resources;
    delete uiManager;
    delete bulletManager;
    delete enemyManager;
    delete player;
    delete levelMap;
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
    pickupManager->Draw();
    enemyManager->Draw();
    player->Draw();
    levelMap->DrawForeground();
    bulletManager->Draw();

    EndMode2D();

    uiManager->DrawHUD(player, waveManager, enemyManager, target.texture.width);
    shopManager.UpdateAndDraw(player, target.texture.width,
                              target.texture.height);

    EndTextureMode();

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
    EndDrawing();
}

void Game::Update() {
    float dt = GetFrameTime();

    if (IsKeyPressed(KEY_B)) {
        shopManager.isOpen = !shopManager.isOpen;
    }

    if (shopManager.isOpen) {
        dt = 0.0f;
    }

    float scale = fminf((float)GetScreenWidth() / virtualWidth,
                        (float)GetScreenHeight() / virtualHeight);

    bulletManager->Update(dt, levelMap);
    player->Update(dt, mousePosition, levelMap, bulletManager);
    // waveManager->Update(dt, enemyManager, player->GetPosition(), levelMap);
    enemyManager->Update(dt, player, levelMap, bulletManager, pickupManager);
    pickupManager->Update(dt, player);

    for(auto& bullet : bulletManager->bullets) {
        if(!bullet.active) continue; 

        if(!bullet.isEnemy) continue;

        if(CheckCollisionRecs(player->GetHitbox(), bullet.GetHitbox())) {
            player->TakeDamage(bullet.damage);
            bullet.active = false;
        }
    }

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
