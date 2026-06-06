#include "Managers/ShopManager.h"
#include "Managers/AudioManager.h"
#include "Managers/ResourceManager.h"
#include "raylib.h"
#include <raygui.h>

ShopManager::ShopManager() {
    upgrades.push_back(
        {"Zelazne Zdrowie", "+20 Max HP", UpgradeType::MAX_HP, 0, 10, 100});

    upgrades.push_back(
        {"Adrenalina", "+10% Szybkosci", UpgradeType::SPEED, 0, 5, 150});
}

void ShopManager::ApplyUpgrade(Player *player, Upgrade &upgrade) {
    player->coins -= upgrade.GetCurrentCost();
    upgrade.currentLevel++;

    switch (upgrade.type) {
    case UpgradeType::MAX_HP:
        player->maxHealth += 20;
        player->health += 20;
        break;
    case UpgradeType::SPEED:
        player->speed *= 1.1f;
        break;
    }
}

void ShopManager::DrawShop(Player *player, ResourceManager *resourceManager,
                           AudioManager *audioManager,
                           const GameBalance &balance) {
    if (!isOpen)
        return;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));

    if (GuiWindowBox(windowBounds, "Sklep")) {
        isOpen = false;
    }

    // --- DEFINIOWANIE MARGINESÓW (PADDINGU) ---
    float padding = 20.0f;
    float headerY = windowBounds.y + 40.0f;
    float startX = windowBounds.x + padding;

    GuiLabel({startX, headerY, 200, 30},
             TextFormat("Kasa: $%d", player->coins));

    // Zakładki (zaczynają się np. w połowie szerokości okna)
    float tabsX = windowBounds.x + (windowBounds.width / 2.0f);
    GuiToggleGroup({tabsX, headerY, windowBounds.width / 4.0f - padding, 30},
                   "Bronie;Postac i Przedmioty", &currentTab);

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
                               WeaponType::AK47, WeaponType::AK47};
        std::string names[4] = {"Pistolet", "MP5", "AK-47", "Shotgun"};
        Texture2D textures[4] = {
            resourceManager->texGlockShoot, resourceManager->texMp5Shoot,
            resourceManager->texAk47Shoot, resourceManager->texAk47Shoot};
        Rectangle texSource[4] = {
            {18, 0, 30, 36}, {0, 0, 64, 36}, {0, 0, 80, 36}, {0, 0, 20, 20}};

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
        int currentPotionCost = balance.player.potionBaseCost;
        if (player->potionsBought > 0) {
            currentPotionCost += (int)(balance.player.potionFlatIncrease * (player->potionsBought * balance.player.potionMultiplier));
        }

        if (GuiButton({120, 140, 300, 40}, TextFormat("Kup Miksture [$%d]", currentPotionCost))) {
            if (player->coins >= currentPotionCost) {
                audioManager->PlayBuy();
                player->coins -= currentPotionCost;
                player->potions++;
                player->potionsBought++;
            }
        }

        bool hpMaxed = (player->maxHealthLevel >= balance.player.maxHealthLevels.size() - 1);
        
        if (!hpMaxed) {
            int hpCost = balance.player.healthUpgradeCost[player->maxHealthLevel];
            float nextHpValue = balance.player.maxHealthLevels[player->maxHealthLevel + 1];
            float hpBonus = nextHpValue - player->maxHealth;
            
            if (GuiButton({120, 190, 300, 40}, TextFormat("Ulepsz Max HP (+%.0f) [$%d]", hpBonus, hpCost))) {
                if (player->coins >= hpCost) {
                    audioManager->PlayBuy();
                    player->coins -= hpCost;
                    player->maxHealthLevel++;
                    
                    // Zwiększamy Max HP z JSON-a i leczymy gracza o dodaną wartość
                    player->maxHealth = balance.player.maxHealthLevels[player->maxHealthLevel];
                    player->health += hpBonus; 
                }
            }
        } else {
            GuiDisable();
            GuiButton({120, 190, 300, 40}, "Max HP [MAX LVL]");
            GuiEnable();
        }

        bool speedMaxed = (player->speedLevel >= balance.player.speedLevels.size() - 1);
        
        if (!speedMaxed) {
            int speedCost = balance.player.speedUpgradeCost[player->speedLevel];
            float nextSpeedValue = balance.player.speedLevels[player->speedLevel + 1];
            float speedBonus = nextSpeedValue - player->speed; // Ile zyskujemy na tym levelu
            
            if (GuiButton({120, 240, 300, 40}, TextFormat("Lepsze Buty (+%.0f SPD) [$%d]", speedBonus, speedCost))) {
                if (player->coins >= speedCost) {
                    audioManager->PlayBuy();
                    player->coins -= speedCost;
                    player->speedLevel++;
                    
                    player->speed = balance.player.speedLevels[player->speedLevel];
                }
            }
        } else {
            GuiDisable();
            GuiButton({120, 240, 300, 40}, "Lepsze Buty [MAX LVL]");
            GuiEnable();
        }

        // Granaty w przyszłości wrzucisz pod współrzędną Y: 290
    }
}

void ShopManager::CalculateNewPrices() {
    potionCost = potionCost + (30 * (float)(potionsBought * 1.1));
}
