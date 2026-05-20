#include "Managers/UIManager.h"
#include "raylib.h"

UIManager::UIManager() {}

void UIManager::DrawHUD(Player *player, WaveManager *waveManager,
                        EnemyManager *enemyManager, int screenWidth) {
    if (!player || !waveManager || !enemyManager)
        return;

    DrawHP(player);
    DrawTime(waveManager, screenWidth);
    DrawWave(waveManager, screenWidth);
    DrawEnemies(enemyManager, screenWidth);
}

void UIManager::DrawWave(WaveManager *waveManager, int screenWidth) {
    const char *waveText = TextFormat("WAVE %d", waveManager->GetCurrentWave());
    int waveFontSize = 20;
    int waveTextWidth = MeasureText(waveText, waveFontSize);

    DrawText(waveText, (screenWidth / 2) - (waveTextWidth / 2) + 2, 42,
             waveFontSize, BLACK);
    DrawText(waveText, (screenWidth / 2) - (waveTextWidth / 2), 40,
             waveFontSize, GOLD);
}

void UIManager::DrawHP(Player *player) {
    float barWidth = 150.0f;
    float barHeight = 15.0f;
    float posX = 10.0f;
    float posY = 10.0f;

    float hpPercent = player->health / player->maxHealth;

    if (hpPercent < 0.0f)
        hpPercent = 0.0f;

    DrawRectangle(posX, posY, barWidth, barHeight, DARKGRAY);
    DrawRectangle(posX, posY, barWidth * hpPercent, barHeight, GREEN);
    DrawRectangleLines(posX, posY, barWidth, barHeight, WHITE);

    DrawText(TextFormat("%d / %d", (int)player->health, (int)player->maxHealth),
             posX + 10, posY + 2, 10, WHITE);
}

void UIManager::DrawTime(WaveManager *waveManager, int screenWidth) {
    int totalSeconds = (int)waveManager->GetGlobalTime();
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    const char *timeText = TextFormat("%02d:%02d", minutes, seconds);

    int timeFontSize = 30;
    int timeTextWidth = MeasureText(timeText, timeFontSize);

    DrawText(timeText, (screenWidth / 2) - (timeTextWidth / 2) + 2, 12,
             timeFontSize, BLACK);
    DrawText(timeText, (screenWidth / 2) - (timeTextWidth / 2), 10,
             timeFontSize, WHITE);
}

void UIManager::DrawEnemies(EnemyManager *enemyManager, int screenWidth) {
    int enemyCount = enemyManager->enemies.size();
    const char *enemyText = TextFormat("ENEMIES: %d", enemyCount);

    int enemyFontSize = 20;
    int enemyTextWidth = MeasureText(enemyText, enemyFontSize);

    DrawText(enemyText, screenWidth - enemyTextWidth - 18, 12, enemyFontSize,
             BLACK);
    DrawText(enemyText, screenWidth - enemyTextWidth - 20, 10, enemyFontSize,
             WHITE);
}
