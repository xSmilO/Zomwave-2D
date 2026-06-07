#include "Managers/ShopManager.h"
#include "Managers/AudioManager.h"
#include "Managers/GameBalance.h"
#include "Managers/ResourceManager.h"
#include "raylib.h"
#include <raygui.h>

ShopManager::ShopManager() {
    upgrades.push_back(
        {"Zelazne Zdrowie", "+20 Max HP", UpgradeType::MAX_HP, 0, 10, 100});

    upgrades.push_back(
        {"Adrenalina", "+10% Szybkosci", UpgradeType::SPEED, 0, 5, 150});
}

void ShopManager::DrawShop(Player *player, ResourceManager *resourceManager,
                           AudioManager *audioManager,
                           const GameBalance &balance) {
    if (!isOpen)
        return;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));

    if (GuiWindowBox(windowBounds, "Sklep")) {
        isOpen = false;
        audioManager->PlayUIClick();
    }

    float padding = 20.0f;
    float headerY = windowBounds.y + 40.0f;
    float startX = windowBounds.x + padding;

    GuiLabel({startX, headerY, 200, 30},
             TextFormat("Kasa: $%d", player->coins));

    float tabsX = windowBounds.x + (windowBounds.width / 2.0f);

    int newTab = currentTab;
    GuiToggleGroup({tabsX, headerY, windowBounds.width / 4.0f - padding, 30},
                   "Bronie;Postac i Przedmioty", &newTab);

    if (newTab != currentTab) {
        audioManager->PlayUIClick();
        currentTab = newTab;
    }

    // =========================================================
    // ZAKŁADKA 0: BRONIE
    // =========================================================
    if (currentTab == 0) {
        float scrollY = headerY + 40.0f;
        float scrollWidth = windowBounds.width - (padding * 2);
        float scrollHeight =
            windowBounds.height - (scrollY - windowBounds.y) - padding;

        Rectangle panelBounds = {startX, scrollY, scrollWidth, scrollHeight};

        Rectangle contentBounds = {startX, scrollY, 1000, scrollHeight - 20};
        Rectangle view = {0};

        GuiScrollPanel(panelBounds, nullptr, contentBounds, &scrollOffset,
                       &view);
        BeginScissorMode((int)view.x, (int)view.y, (int)view.width,
                         (int)view.height);

        float tileWidth = 200.0f;
        float tileHeight = contentBounds.height - 20.0f;
        float startTileX = contentBounds.x + 10.0f;
        float tileY = contentBounds.y + 10.0f;
        float spacing = tileWidth + 20.0f;

        WeaponType types[4] = {WeaponType::GLOCK, WeaponType::MP5,
                               WeaponType::SHOTGUN, WeaponType::AK47};
        std::string names[4] = {"Pistolet", "MP5", "Shotgun", "AK-47"};
        Texture2D textures[4] = {
            resourceManager->texGlockShoot, resourceManager->texMp5Shoot,
            resourceManager->texShotgunShoot, resourceManager->texAk47Shoot};
        Rectangle texSource[4] = {
            {18, 0, 30, 36}, {0, 0, 64, 36}, {0, 48, 128, 128}, {0, 0, 80, 36}};

        for (int i = 0; i < 4; i++) {

            float currentX = startTileX + (i * spacing) + scrollOffset.x;
            Rectangle tileRec = {currentX, tileY, tileWidth, tileHeight};

            if (CheckCollisionRecs(tileRec, view)) {

                Weapon *weapon = player->GetWeapon(types[i]);

                GuiGroupBox(tileRec, TextFormat("%s lvl %d", names[i].c_str(),
                                                weapon->currentLevel));
                if (weapon != nullptr) {

                    float texScale = 2.0f;
                    float texW = texSource[i].width * texScale;
                    float texH = texSource[i].height * texScale;

                    Rectangle dest = {currentX + (tileWidth / 2.0f) -
                                          (texW / 2.0f),
                                      tileY + 40, texW, texH};

                    Color drawColor = weapon->isUnlocked ? WHITE : DARKGRAY;

                    DrawTexturePro(textures[i], texSource[i], dest, {0, 0},
                                   0.0f, drawColor);

                    float btnWidth = tileWidth - 20.0f;
                    float btnX = currentX + 10.0f;

                    if (!weapon->isUnlocked) {

                        Rectangle btnBuy = {btnX, tileY + tileHeight - 50,
                                            btnWidth, 40};
                        if (CheckCollisionRecs(btnBuy, view)) {
                            if (GuiButton(btnBuy,
                                          TextFormat("KUP [$%d]",
                                                     weapon->unlockCost))) {
                                if (player->coins >= weapon->unlockCost) {
                                    audioManager->PlayBuy();
                                    player->coins -= weapon->unlockCost;
                                    weapon->isUnlocked = true;
                                    weapon->currentLevel = 1;
                                    weapon->LoadStatsFromBalance(balance);
                                }
                            }
                        }
                    } else {
                        Rectangle btnEquip = {btnX, tileY + tileHeight - 100,
                                              btnWidth, 40};
                        Rectangle btnUpgrade = {btnX, tileY + tileHeight - 50,
                                                btnWidth, 40};

                        if (CheckCollisionRecs(btnEquip, view)) {

                            bool isEquipped =
                                (player->GetActiveWeapon()->type == types[i]);
                            const char *equipText =
                                isEquipped ? "W REKU" : "WEZ DO REKI";

                            if (GuiButton(btnEquip, equipText) && !isEquipped) {
                                audioManager->PlayUIClick();
                                player->EquipWeapon(types[i]);
                            }
                        }
                        if (CheckCollisionRecs(btnUpgrade, view)) {
                            bool maxLvl =
                                weapon->currentLevel == weapon->maxLevel;
                            const char *upgradeText =
                                maxLvl ? "MAX LVL" : "ULEPSZ [$%d]";

                            if (maxLvl)
                                GuiDisable();

                            if (GuiButton(btnUpgrade,
                                          TextFormat(upgradeText,
                                                     weapon->upgradeCost))) {
                                if (player->coins >= weapon->upgradeCost) {
                                    audioManager->PlayBuy();
                                    player->coins -= weapon->upgradeCost;

                                    weapon->currentLevel++;
                                    weapon->LoadStatsFromBalance(balance);
                                }
                            }

                            if (maxLvl)
                                GuiEnable();
                        }
                    }
                }
            }
        }
        EndScissorMode();
    }
    // =========================================================
    // ZAKŁADKA 1: POSTAĆ I PRZEDMIOTY
    // =========================================================
    else if (currentTab == 1) {

        // --- 1. USTAWIENIA SCROLL PANELA (Kopia z currentTab == 0) ---
        float scrollY = headerY + 40.0f;
        float scrollWidth = windowBounds.width - (padding * 2);
        float scrollHeight =
            windowBounds.height - (scrollY - windowBounds.y) - padding;

        Rectangle panelBounds = {startX, scrollY, scrollWidth, scrollHeight};
        Rectangle contentBounds = {
            startX, scrollY, 1000,
            scrollHeight - 20}; // 1000px szerokości styknie na 3 kolumny
        Rectangle view = {0};

        GuiScrollPanel(panelBounds, nullptr, contentBounds, &scrollOffset,
                       &view);
        BeginScissorMode((int)view.x, (int)view.y, (int)view.width,
                         (int)view.height);

        // --- 2. DYNAMICZNE WYMIARY KART ---
        float cardWidth = 220.0f;
        float statsWidth = 320.0f;
        float cardHeight = contentBounds.height - 20.0f;

        float startTileX = contentBounds.x + 10.0f;
        float tileY = contentBounds.y + 10.0f;

        // X z uwzględnieniem offsetu ze scrollbara
        float col1_X = startTileX + scrollOffset.x;
        float col2_X = col1_X + cardWidth + 20.0f;
        float col3_X = col2_X + cardWidth + 40.0f;

        // ==========================================
        // KOLUMNA 1: MIKSTURA ZDROWIA
        // ==========================================
        Rectangle potRec = {col1_X, tileY, cardWidth, cardHeight};
        if (CheckCollisionRecs(potRec, view)) {
            GuiGroupBox(potRec, "Mikstura Zdrowia");

            int currentPotionCost = balance.player.potionBaseCost;
            if (player->potionsBought > 0) {
                currentPotionCost += (int)(balance.player.potionFlatIncrease *
                                           (player->potionsBought *
                                            balance.player.potionMultiplier));
            }

            Texture2D texPot = resourceManager->texHealthPotion;
            float potScale = 3.0f;
            float potW = 32 * potScale;
            float potH = 32 * potScale;
            Rectangle potDest = {col1_X + (cardWidth / 2.0f) - (potW / 2.0f),
                                 tileY + 40.0f, potW, potH};
            DrawTexturePro(texPot,
                           {0, 0, (float)texPot.width, (float)texPot.height},
                           potDest, {0, 0}, 0.0f, WHITE);

            DrawText(TextFormat("W plecaku: %d", player->potions),
                     (int)col1_X + 50, (int)(tileY + cardHeight - 80), 15,
                     SKYBLUE);

            Rectangle btnBuyPot = {col1_X + 10.0f, tileY + cardHeight - 50.0f,
                                   cardWidth - 20.0f, 40.0f};
            if (CheckCollisionRecs(
                    btnBuyPot,
                    view)) { 
                if (GuiButton(btnBuyPot,
                              TextFormat("KUP [$%d]", currentPotionCost))) {
                    if (player->coins >= currentPotionCost) {
                        audioManager->PlayBuy();
                        player->coins -= currentPotionCost;
                        player->potions++;
                        player->potionsBought++;
                    }
                }
            }
        }

        // ==========================================
        // KOLUMNA 2: GRANAT ODŁAMKOWY
        // ==========================================
        Rectangle bombRec = {col2_X, tileY, cardWidth, cardHeight};
        if (CheckCollisionRecs(bombRec, view)) {
            GuiGroupBox(bombRec, "Bomba");

            Texture2D texBomb = resourceManager->texBomb;
            float bombScale = 3.0f;
            float bW = 32 * bombScale;
            float bH = 32 * bombScale;
            Rectangle bombDest = {col2_X + (cardWidth / 2.0f) - (bW / 2.0f),
                                  tileY + 40.0f, bW, bH};
            DrawTexturePro(texBomb,
                           {0, 0, (float)texBomb.width, (float)texBomb.height},
                           bombDest, {0, 0}, 0.0f, WHITE);

            DrawText(TextFormat("W plecaku: %d", player->bombs),
                     (int)col2_X + 50, (int)(tileY + cardHeight - 80), 15,
                     ORANGE);

            Rectangle btnBuyBomb = {col2_X + 10.0f, tileY + cardHeight - 50.0f,
                                    cardWidth - 20.0f, 40.0f};
            if (CheckCollisionRecs(btnBuyBomb, view)) {
                if (GuiButton(btnBuyBomb, TextFormat("KUP [$%d]", balance.bomb.cost))) {
                    if (player->coins >= balance.bomb.cost) {
                        audioManager->PlayBuy();
                        player->coins -= balance.bomb.cost;
                        player->bombs++;
                    }
                }
            }
        }

        // ==========================================
        // KOLUMNA 3: ULEPSZENIA STATYSTYK
        // ==========================================
        Rectangle statsRec = {col3_X, tileY, statsWidth, cardHeight};
        if (CheckCollisionRecs(statsRec, view)) {
            GuiGroupBox(statsRec, "Ulepszenia Postaci");

            float btnX = col3_X + 15.0f;
            float btnW = statsWidth - 30.0f;

            // Dynamiczne wyliczenie odstępów, żeby ładnie wypełniły wysokość
            // karty
            float currentY = tileY + 30.0f;
            float ySpacing = (cardHeight - 40.0f) / 3.0f;

            // --- 1. MAX HP ---
            Rectangle btnHpRec = {btnX, currentY, btnW, 40.0f};
            if (CheckCollisionRecs(btnHpRec, view)) {
                bool hpMaxed = (player->maxHealthLevel >=
                                balance.player.maxHealthLevels.size() - 1);
                if (!hpMaxed) {
                    int hpCost = balance.player
                                     .healthUpgradeCost[player->maxHealthLevel];
                    float hpBonus =
                        balance.player
                            .maxHealthLevels[player->maxHealthLevel + 1] -
                        player->maxHealth;

                    if (GuiButton(btnHpRec, TextFormat("Max HP (+%.0f) [$%d]",
                                                       hpBonus, hpCost))) {
                        if (player->coins >= hpCost) {
                            audioManager->PlayBuy();
                            player->coins -= hpCost;
                            player->maxHealthLevel++;
                            player->maxHealth =
                                balance.player
                                    .maxHealthLevels[player->maxHealthLevel];
                            player->health += hpBonus;
                        }
                    }
                } else {
                    GuiDisable();
                    GuiButton(btnHpRec, "Max HP [MAX LVL]");
                    GuiEnable();
                }
            }

            currentY += ySpacing;

            // --- 2. SZYBKOŚĆ (BUTY) ---
            Rectangle btnSpeedRec = {btnX, currentY, btnW, 40.0f};
            if (CheckCollisionRecs(btnSpeedRec, view)) {
                bool speedMaxed = (player->speedLevel >=
                                   balance.player.speedLevels.size() - 1);
                if (!speedMaxed) {
                    int speedCost =
                        balance.player.speedUpgradeCost[player->speedLevel];
                    float speedBonus =
                        balance.player.speedLevels[player->speedLevel + 1] -
                        player->speed;

                    if (GuiButton(btnSpeedRec,
                                  TextFormat("Lepsze Buty (+%.0f SPD) [$%d]",
                                             speedBonus, speedCost))) {
                        if (player->coins >= speedCost) {
                            audioManager->PlayBuy();
                            player->coins -= speedCost;
                            player->speedLevel++;
                            player->speed =
                                balance.player.speedLevels[player->speedLevel];
                        }
                    }
                } else {
                    GuiDisable();
                    GuiButton(btnSpeedRec, "Lepsze Buty [MAX LVL]");
                    GuiEnable();
                }
            }

            currentY += ySpacing;

            // --- 3. WIZJA (LATARKA) ---
            Rectangle btnVisRec = {btnX, currentY, btnW, 40.0f};
            if (CheckCollisionRecs(btnVisRec, view)) {
                bool visionMaxed = (player->visionLevel >=
                                    balance.player.visionLevels.size() - 1);
                if (!visionMaxed) {
                    int visionCost =
                        balance.player.visionUpgradeCost[player->visionLevel];
                    float visionBonus =
                        balance.player.visionLevels[player->visionLevel + 1] -
                        player->visionRadius;

                    if (GuiButton(btnVisRec,
                                  TextFormat("Lepsza Latarka (+%.0f) [$%d]",
                                             visionBonus, visionCost))) {
                        if (player->coins >= visionCost) {
                            audioManager->PlayBuy();
                            player->coins -= visionCost;
                            player->visionLevel++;
                            player->visionRadius =
                                balance.player
                                    .visionLevels[player->visionLevel];
                        }
                    }
                } else {
                    GuiDisable();
                    GuiButton(btnVisRec, "Lepsza Latarka [MAX LVL]");
                    GuiEnable();
                }
            }
        }

        EndScissorMode(); // Koniec maskowania dla całego scroll panela
    }
}

void ShopManager::CalculateNewPrices() {
    potionCost = potionCost + (30 * (float)(potionsBought * 1.1));
}
