#include "Managers/ShopManager.h"
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

void ShopManager::DrawShop(Player *player, ResourceManager *resourceManager) {
    if (!isOpen)
        return;

    // 1. Tło pauzy
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));

    // 2. Główne okno korzysta z naszej zmiennej bazowej!
    if (GuiWindowBox(windowBounds, "Sklep u Zombiaka")) {
        isOpen = false;
    }

    // --- DEFINIOWANIE MARGINESÓW (PADDINGU) ---
    float padding = 20.0f;
    float headerY =
        windowBounds.y +
        40.0f; // 40px w dół od góry okna (zostawiamy miejsce na pasek "X")
    float startX =
        windowBounds.x +
        padding; // Wszystko wewnątrz zaczyna się od lewej z marginesem

    // Etykieta kasy
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

        // --- ZMIANA NA SCROLL POZIOMY ---
        // Szerokość to np. 1000 pikseli, a wysokość odejmujemy o 20px,
        // żeby zrobić miejsce na sam pasek scrollbara na dole i uniknąć
        // pionowego scrolla!
        Rectangle contentBounds = {startX, scrollY, 1000, scrollHeight - 20};
        Rectangle view = {0};

        GuiScrollPanel(panelBounds, nullptr, contentBounds, &scrollOffset,
                       &view);
        BeginScissorMode((int)view.x, (int)view.y, (int)view.width,
                         (int)view.height);

        // --- WYMIARY KAFELKÓW (KARTY) ---
        float tileWidth = 200.0f; // Węższe, jak karty
        float tileHeight =
            contentBounds.height -
            20.0f; // Rozciągnięte na prawie całą wysokość scrolla
        float startTileX = contentBounds.x + 10.0f;
        float tileY = contentBounds.y + 10.0f; // Y jest teraz stałe!
        float spacing = tileWidth + 20.0f;     // Odstęp w poziomie

        WeaponType types[4] = {WeaponType::PISTOL, WeaponType::SMG,
                               WeaponType::ASSAULT_RIFLE,
                               WeaponType::ASSAULT_RIFLE};
        std::string names[4] = {"Pistolet", "MP5", "AK-47", "Shotgun"};
        Texture2D textures[4] = {
            resourceManager->texPistolShoot, resourceManager->texMp5Shoot,
            resourceManager->texAk47Shoot, resourceManager->texAk47Shoot};
        Rectangle texSource[4] = {
            {20, 0, 30, 36}, {0, 0, 64, 36}, {0, 0, 80, 36}, {0, 0, 20, 20}};

        for (int i = 0; i < 4; i++) {

            // MAGIA HORYZONTALNA: Używamy scrollOffset.x i zmieniamy currentX!
            float currentX = startTileX + (i * spacing) + scrollOffset.x;
            Rectangle tileRec = {currentX, tileY, tileWidth, tileHeight};

            if (CheckCollisionRecs(tileRec, view)) {

                Weapon *weapon = player->GetWeapon(types[i]);

                GuiGroupBox(tileRec, TextFormat("%s lvl %d", names[i].c_str(),
                                                weapon->currentLevel));
                if (weapon != nullptr) {

                    // --- CENTROWANIE TEKSTURY (Większa i wyżej) ---
                    float texScale = 2.0f;
                    float texW = texSource[i].width * texScale;
                    float texH = texSource[i].height * texScale;

                    Rectangle dest = {currentX + (tileWidth / 2.0f) -
                                          (texW / 2.0f),
                                      tileY + 40, texW, texH};

                    Color drawColor = weapon->isUnlocked ? WHITE : DARKGRAY;
                    // DrawRectangle(dest.x, dest.y, texW, texH, RED);
                    DrawTexturePro(textures[i], texSource[i], dest, {0, 0},
                                   0.0f, drawColor);

                    // --- PRZYCISKI (Ułożone pionowo na dole karty) ---
                    float btnWidth =
                        tileWidth - 20.0f; // Szerokość dopasowana do karty
                    float btnX = currentX + 10.0f; // Wyśrodkowane w karcie

                    if (!weapon->isUnlocked) {
                        // Przycisk ZAKUPU na samym dole
                        Rectangle btnBuy = {btnX, tileY + tileHeight - 50,
                                            btnWidth, 40};
                        if (CheckCollisionRecs(btnBuy, view)) {
                            if (GuiButton(btnBuy,
                                          TextFormat("KUP [$%d]",
                                                     weapon->unlockCost))) {
                                if (player->coins >= weapon->unlockCost) {
                                    player->coins -= weapon->unlockCost;
                                    weapon->isUnlocked = true;
                                }
                            }
                        }
                    } else {
                        // Przyciski WEŹ i ULEPSZ jeden pod drugim!
                        Rectangle btnEquip = {btnX, tileY + tileHeight - 100,
                                              btnWidth, 40};
                        Rectangle btnUpgrade = {btnX, tileY + tileHeight - 50,
                                                btnWidth, 40};

                        if (CheckCollisionRecs(btnEquip, view)) {
                            // Możemy zmienić kolor/tekst, jeśli to aktualnie
                            // trzymana broń
                            bool isEquipped =
                                (player->GetActiveWeapon()->type == types[i]);
                            const char *equipText =
                                isEquipped ? "W REKU" : "WEZ DO REKI";

                            if (GuiButton(btnEquip, equipText) && !isEquipped) {
                                player->EquipWeapon(types[i]);
                            }
                        }
                        if (CheckCollisionRecs(btnUpgrade, view)) {
                            bool maxLvl =
                                weapon->currentLevel == weapon->maxLevel;
                            const char *upgradeText =
                                maxLvl ? "MAX LVL" : "ULEPSZ BRON [$%d]";
                            if (GuiButton(btnUpgrade,
                                          TextFormat(upgradeText,
                                                     weapon->upgradeCost)) &&
                                !maxLvl) {
                                if (player->coins >= weapon->upgradeCost) {
                                    player->coins -= weapon->upgradeCost;
                                    weapon->damage += 2;
                                    weapon->currentLevel++;
                                }
                            }
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

        // POTKA: Leczenie
        if (GuiButton({120, 140, 300, 40}, "Wypij Miksture (+50 HP) [$50]")) {
            if (player->coins >= 50 && player->health < player->maxHealth) {
                player->coins -= 50;
                player->health += 50;
                if (player->health > player->maxHealth)
                    player->health = player->maxHealth;
            }
        }

        // ULEPSZENIE: Max HP
        if (GuiButton({120, 190, 300, 40},
                      TextFormat("Ulepsz Max HP (+20) [$150] (Aktualne: %d)",
                                 player->maxHealth))) {
            if (player->coins >= 150) {
                player->coins -= 150;
                player->maxHealth += 20;
                player->health += 20; // Dajemy graczowi to HP od razu, żeby nie
                                      // czuł się oszukany
            }
        }

        // ULEPSZENIE: Movement Speed
        if (GuiButton(
                {120, 240, 300, 40},
                TextFormat("Lepsze Buty (Szybkosc +10) [$200] (Akt: %.0f)",
                           player->speed))) {
            if (player->coins >= 200) {
                player->coins -= 200;
                player->speed += 10.0f;
            }
        }

        // Granaty w przyszłości wrzucisz pod współrzędną Y: 290
    }
}
