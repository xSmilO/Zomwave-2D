#include "Managers/ShopManager.h"
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

void ShopManager::UpdateAndDraw(Player *player, int screenWidth,
                                int screenHeight) {
    if (!isOpen)
        return;

    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.8f));

    int panelWidth = 700;
    int panelHeight = 550;
    int startX = (screenWidth - panelWidth) / 2;
    int startY = (screenHeight - panelHeight) / 2;

    if (GuiWindowBox({(float)startX, (float)startY, (float)panelWidth,
                      (float)panelHeight},
                     "SKLEP I ZBROJOWNIA")) {
        isOpen = false;
    }

    DrawText(TextFormat("Monety: %d $", player->coins), startX + 20,
             startY + 40, 20, GOLD);

    bool tab1 = (activeTab == 0);
    if (GuiToggle({(float)startX + 250, (float)startY + 30, 120, 30},
                  "ZBROJOWNIA", &tab1))
        activeTab = 0;

    bool tab2 = (activeTab == 0);
    if (GuiToggle({(float)startX + 370, (float)startY + 30, 120, 30}, "POSTAC",
                  &tab2))
        activeTab = 1;

    int itemY = startY = 80;

    if (activeTab == 0) {
    }
}
