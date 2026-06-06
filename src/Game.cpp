#include "Game.h"
#include "Managers/BulletManager.h"
#include "Managers/ConfigManager.h"
#include "Managers/EnemyManager.h"
#include "Managers/UIManager.h"
#include "Managers/WaveManager.h"
#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <raygui.h>

Game::Game() {
#if defined(PLATFORM_WEB)
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
#else
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT |
                   FLAG_WINDOW_RESIZABLE); // set custom flags for desktop
    SetTargetFPS(
        144); // do not set fps when used in browser for better web performance
#endif
    previousState = GameState::MAIN_MENU;
    currentState = GameState::MAIN_MENU;
    screenWidth = 1920;
    screenHeight = 1080;
    camera.offset = {virtualWidth / 2.0f, virtualHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    InitWindow(screenWidth, screenHeight, "Zomwave 2D");
    InitAudioDevice();
    SetExitKey(KEY_NULL);

    resources = new ResourceManager();
    resources->LoadAll();

    audioManager.Init(resources);

    pickupManager =
        new PickupManager(&resources->texHealthPotion, &resources->texCoin);

    uiManager = new UIManager(&resources->texHealthPotion, &resources->texCoin);

    bulletManager =
        new BulletManager(&resources->texBullet, &resources->texArrow);
    enemyManager = new EnemyManager(resources, bulletManager, &audioManager);
    waveManager = new WaveManager();

    target = LoadRenderTexture(virtualWidth, virtualHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    levelMap = new Map();
    player = new Player(resources, &audioManager);

    SpawnPlayer();
    ConfigManager::LoadSettings(audioManager, currentFpsIndex, fpsOptions);
    audioManager.PlayMenuMusic();
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
    audioManager.CleanUp();
    CloseWindow();
}

void Game::Draw() {
    // set proper width and height in browser window
#if defined(PLATFORM_WEB)
    int padding = 30; // set padding to avoid scrollbar and browser edge overlap
    SetWindowSize(getBrowserWidth() - padding, getBrowserHeight() - padding);
#endif

    switch (currentState) {
    case GameState::MAIN_MENU:
        DrawMainMenu();
        break;
    case GameState::PLAYING:
        DrawPlaying();
        break;
    case GameState::SETTINGS:
        DrawSettings();
        break;
    case GameState::PAUSED:
        DrawPaused();
        break;
    case GameState::EXIT:
        break;
    }

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
    float scale = fminf((float)GetScreenWidth() / virtualWidth,
                        (float)GetScreenHeight() / virtualHeight);
    float offsetX = (GetScreenWidth() - (virtualWidth * scale)) * 0.5f;
    float offsetY = (GetScreenHeight() - (virtualHeight * scale)) * 0.5f;

    SetMouseOffset(-(int)offsetX, -(int)offsetY);
    SetMouseScale(1.0f / scale, 1.0f / scale);

    float dt = GetFrameTime();

    audioManager.UpdateMusic();

    BeginTextureMode(target);
    ClearBackground(RAYWHITE);

    switch (currentState) {
    case GameState::MAIN_MENU:
        UpdateMainMenu(dt);
        break;
    case GameState::PLAYING:
        UpdatePlaying(dt);
        break;
    case GameState::SETTINGS:
        UpdateSettings(dt);
        break;
    case GameState::PAUSED:
        UpdatePaused(dt);
        break;
    case GameState::EXIT:
        // CloseWindow();
        break;
    }
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
    while (!WindowShouldClose() && currentState != GameState::EXIT) {
        MainLoopHelper(this);
    }
#endif
}

void Game::UpdateMainMenu(float dt) {}

void Game::UpdatePlaying(float dt) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentState = GameState::PAUSED;
        audioManager.PauseMusic();
    }

    if (IsKeyPressed(KEY_B)) {
        shopManager.isOpen = !shopManager.isOpen;
    }

    if (shopManager.isOpen) {
        dt = 0.0f;
        return;
    }

    bulletManager->Update(dt, levelMap);
    player->Update(dt, mousePosition, levelMap, bulletManager);
    waveManager->Update(dt, enemyManager, player->GetPosition(), levelMap);
    enemyManager->Update(dt, player, levelMap, bulletManager, pickupManager);
    pickupManager->Update(dt, player, &audioManager);

    for (auto &bullet : bulletManager->bullets) {
        if (!bullet.active)
            continue;

        if (!bullet.isEnemy)
            continue;

        if (CheckCollisionRecs(player->GetHitbox(), bullet.GetHitbox())) {
            player->TakeDamage(bullet.damage);
            bullet.active = false;
        }
    }

    camera.target = player->GetPosition();
    Vector2 mouseScreen = GetMousePosition();

    mousePosition.x = Clamp(mouseScreen.x, 0, virtualWidth);
    mousePosition.y = Clamp(mouseScreen.y, 0, virtualHeight);

    mousePosition = GetScreenToWorld2D(mousePosition, camera);
}

void Game::UpdateSettings(float dt) {}

void Game::UpdatePaused(float dt) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentState = GameState::PLAYING;
        audioManager.ResumeMusic();
    }
}

void Game::DrawMainMenu() {
    DrawTexturePro(resources->texSkyBox,
                   {0, 0, (float)resources->texSkyBox.width,
                    (float)resources->texSkyBox.height},
                   {0, 0, (float)virtualWidth, (float)virtualHeight}, {0, 0}, 0,
                   WHITE);

    float btnWidth = 200.0f;
    float btnHeight = 50.0f;
    float spacing = 20.0f;

    float centerX = virtualWidth / 2.0f;
    float btnX = centerX - (btnWidth / 2.0f);
    float startY = virtualHeight / 2.0f - 50.0f;

    const char *title = "Zomwave 2D";
    int titleSize = 60;
    int titleWidth = MeasureText(title, titleSize);
    DrawText(title, centerX - (titleWidth / 2), 100, titleSize, MAROON);

    if (GuiButton({btnX, startY, btnWidth, btnHeight}, "GRAJ")) {
        audioManager.PlayUIClick();
        currentState = GameState::PLAYING;
        audioManager.PlayGameMusic();
    }
    if (GuiButton({btnX, startY + (btnHeight + spacing), btnWidth, btnHeight},
                  "USTAWIENIA")) {
        audioManager.PlayUIClick();
        previousState = currentState;
        currentState = GameState::SETTINGS;
    }

    if (GuiButton(
            {btnX, startY + (btnHeight + spacing) * 2, btnWidth, btnHeight},
            "WYJDZ")) {
        audioManager.PlayUIClick();
        currentState = GameState::EXIT;
    }
}

void Game::DrawPlaying() {
    DrawTexturePro(resources->texSkyBox,
                   {0, 0, (float)resources->texSkyBox.width,
                    (float)resources->texSkyBox.height},
                   {0, 0, (float)virtualWidth, (float)virtualHeight}, {0, 0}, 0,
                   WHITE);

    BeginMode2D(camera);
    levelMap->DrawBackground();
    pickupManager->Draw();
    enemyManager->Draw();
    player->Draw();
    levelMap->DrawForeground();
    bulletManager->Draw();

    EndMode2D();

    uiManager->DrawHUD(player, waveManager, enemyManager, target.texture.width);

    shopManager.DrawShop(player, resources, &audioManager);
}

void Game::DrawSettings() {
    float startPos = 20;
    float marginTop = 80;

    DrawRectangle(0, 0, virtualWidth, virtualHeight, {20, 20, 20, 255});

    float centerX = virtualWidth / 2.0f;
    float btnWidth = 300.0f;
    float btnX = centerX - (btnWidth / 2.0f);

    DrawText("USTAWIENIA", centerX - MeasureText("USTAWIENIA", 50) / 2,
             startPos, 50, LIGHTGRAY);

    const char *musText =
        TextFormat("MUZYKA: %i%%", (int)(audioManager.GetMusicVolume() * 100));
    DrawText(musText, centerX - MeasureText(musText, 25) / 2,
             startPos + marginTop, 25, GRAY);

    float tempMusicVol = audioManager.GetMusicVolume();

    GuiSlider({btnX, startPos + marginTop + 40, btnWidth, 40}, "0%", "100%",
              &tempMusicVol, 0.0f, 1.0f);

    if (tempMusicVol != audioManager.GetMusicVolume()) {
        audioManager.SetMusicVolume(tempMusicVol);
    }

    const char *sfxText = TextFormat("EFEKTY (SFX): %i%%",
                                     (int)(audioManager.GetSFXVolume() * 100));
    DrawText(sfxText, centerX - MeasureText(sfxText, 25) / 2,
             startPos + (marginTop * 2) + 20, 25, GRAY);

    float tempSfxVol = audioManager.GetSFXVolume();
    GuiSlider({btnX, startPos + (marginTop * 2) + 60, btnWidth, 40}, "0%",
              "100%", &tempSfxVol, 0.0f, 1.0f);

    if (tempSfxVol != audioManager.GetSFXVolume()) {
        audioManager.SetSFXVolume(tempSfxVol);
    }

    const char *fpsText =
        TextFormat("LIMIT FPS: %i", fpsOptions[currentFpsIndex]);

    if (GuiButton({btnX, startPos + (marginTop * 3) + 40, btnWidth, 50},
                  fpsText)) {
        currentFpsIndex++; // Zwiększamy indeks

        if (currentFpsIndex >= fpsOptions.size()) {
            currentFpsIndex = 0;
        }

        audioManager.PlayUIClick();
        SetTargetFPS(fpsOptions[currentFpsIndex]);
    }

    if (GuiButton({centerX - 100.0f, startPos + (marginTop * 4) + 40, 200, 50},
                  "WROC")) {
        ConfigManager::SaveSettings(audioManager, currentFpsIndex);
        audioManager.PlayUIClick();
        currentState = previousState;
    }
}

void Game::DrawPaused() {
    DrawPlaying();

    DrawRectangle(0, 0, virtualWidth, virtualHeight, {0, 0, 0, 150});

    int screenWidth = virtualWidth;
    float btnWidth = 200.0f;
    float btnX = screenWidth / 2.0f - (btnWidth / 2.0f);

    DrawText("PAUZA", screenWidth / 2 - MeasureText("PAUZA", 60) / 2, 50, 60,
             LIGHTGRAY);

    if (GuiButton({btnX, 170, btnWidth, 50}, "WROC DO GRY")) {
        audioManager.PlayUIClick();
        currentState = GameState::PLAYING;
        audioManager.ResumeMusic();
    }

    if (GuiButton({btnX, 240, btnWidth, 50}, "USTAWIENIA")) {
        audioManager.PlayUIClick();
        previousState = currentState;
        currentState = GameState::SETTINGS;
    }

    if (GuiButton({btnX, 310, btnWidth, 50}, "MENU GLOWNE")) {
        audioManager.PlayUIClick();
        currentState = GameState::MAIN_MENU;
        audioManager.PlayMenuMusic();
    }
}

void Game::SpawnPlayer() {
    player->isAlive = true;
    player->SetPosition(levelMap->GetSpawnPoint());
}
