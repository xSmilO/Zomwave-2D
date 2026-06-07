#include "Managers/BombManager.h"
#include "Managers/GameBalance.h"

void BombManager::ThrowBomb(Vector2 start, Vector2 target) {
    Bomb b;
    b.startPos = start;
    b.currentPos = start;
    b.targetPos = target;
    b.flightDuration = 0.6f;
    b.flightTimer = b.flightDuration;
    bombs.push_back(b);
}

void BombManager::Update(float dt, Player* player, EnemyManager *enemyManager,
                         AudioManager *audioManager) {
    for (auto &bomb : bombs) {
        if (bomb.isExploding) {

            bomb.explosionTimer -= dt;
            continue;
        }

        if (bomb.flightTimer > 0) {

            bomb.flightTimer -= dt;
            float t = 1.0f - (bomb.flightTimer / bomb.flightDuration);

            bomb.currentPos = Vector2Lerp(bomb.startPos, bomb.targetPos, t);
        } else {

            bomb.isExploding = true;
            bomb.explosionTimer = 0.3f;

            audioManager->PlayBombExplosion(bomb.currentPos, player->GetPosition());

            for (auto &enemy : enemyManager->enemies) {
                if (enemy->active) {
                    float dist =
                        Vector2Distance(bomb.currentPos, enemy->position);
                    if (dist <= explosionRadius) {
                        enemy->TakeDamage(damage);
                    }
                }
            }
        }
    }

    bombs.erase(std::remove_if(bombs.begin(), bombs.end(),
                               [](const Bomb &b) {
                                   return b.isExploding &&
                                          b.explosionTimer <= 0.0f;
                               }),
                bombs.end());
}

void BombManager::Draw(Texture2D texBomb) {
    for (const auto &bomb : bombs) {
        if (bomb.isExploding) {
            float t = 1.0f - (bomb.explosionTimer / 0.3f);
            float currentRadius = explosionRadius * t;
            Color explColor =
                Color{255, 100, 0, (unsigned char)(255 * (1.0f - t))};
            DrawCircleV(bomb.currentPos, currentRadius, explColor);
            DrawCircleV(bomb.currentPos, currentRadius * 0.5f,
                        Color{255, 200, 50, (unsigned char)(200 * (1.0f - t))});
        } else {
            float t = 1.0f - (bomb.flightTimer / bomb.flightDuration);

            float heightScale = 1.0f + sinf(t * PI) * 1.5f;
            float rotation = t * 360.0f * 4.0f;

            Rectangle sourceRec = {0.0f, 0.0f, (float)texBomb.width,
                                   (float)texBomb.height};

            float scaledWidth = 16 * heightScale;
            float scaledHeight = 16 * heightScale;
            Rectangle destRec = {bomb.currentPos.x, bomb.currentPos.y,
                                 scaledWidth, scaledHeight};

            Vector2 origin = {scaledWidth / 2.0f, scaledHeight / 2.0f};

            DrawTexturePro(texBomb, sourceRec, destRec, origin, rotation,
                           WHITE);
        }
    }
}
