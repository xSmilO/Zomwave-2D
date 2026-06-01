#include "Entities/Enemy.h"

void Enemy::DrawHealthBar() {
    float currentHealth = (health < 0) ? 0 : health;
    float hpPercent = (float)currentHealth / maxHealth;

    float barWidth = width;
    float barHeight = 6.0f;

    Vector2 barPos = {position.x - (barWidth / 2.0f), position.y - 30.0f};
    DrawRectangle(barPos.x, barPos.y, barWidth, barHeight, RED);

    DrawRectangle(barPos.x, barPos.y, barWidth * hpPercent, barHeight, GREEN);
    DrawRectangleLines(barPos.x, barPos.y, barWidth, barHeight, BLACK);
}
