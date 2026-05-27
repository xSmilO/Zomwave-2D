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

void ShopManager::DrawShop(Player *player) {
    if (!isOpen)
        return;

    // 1. Tło pauzy
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));

    // 2. Główne okno (Zwraca true, jeśli klikniesz "X")
    if (GuiWindowBox({100, 50, 600, 500}, "Sklep u Zombiaka")) {
        isOpen = false;
    }

    // Wyświetlanie kasy
    GuiLabel({120, 90, 200, 30},
             TextFormat("Twoje srodki: $%d", player->coins));

    // =========================================================
    // 3. ZAKŁADKI (Zmienia wartość 'currentTab' po kliknięciu)
    // =========================================================
    GuiToggleGroup({350, 90, 150, 30}, "Bronie;Postac i Przedmioty",
                   &currentTab);

    // =========================================================
    // ZAKŁADKA 0: BRONIE
    // =========================================================
    if (currentTab == 0) {

        // --- KAFELEK 1: SHOTGUN ---
        GuiGroupBox({120, 140, 250, 150}, "MP5");

        // Pobieramy wskaźnik na Shotguna (zakładam, że masz metodę
        // player->GetWeapon)
        Weapon *mp5 = player->GetWeapon(WeaponType::SMG);

        if (mp5 != nullptr) {

            // Sytuacja A: Broń jest jeszcze ZABLOKOWANA
            if (!mp5->isUnlocked) {

                if (GuiButton({130, 230, 230, 50}, "ODBLOKUJ MP5 [$500]")) {
                    if (player->coins >= 500) {
                        player->coins -= 500;
                        mp5->isUnlocked =
                            true; // Po prostu zmieniasz flagę na true!
                    }
                }
            }
            // Sytuacja B: Broń jest już ODBLOKOWANA
            else {

                // Przycisk wyekwipowania broni
                if (GuiButton({130, 170, 230, 40}, "WEZ DO REKI")) {
                    player->EquipWeapon(WeaponType::SHOTGUN);
                }
                // Przycisk ulepszania statystyk
                if (GuiButton({130, 230, 230, 40}, "ULEPSZ DMG (+2) [$200]")) {
                    if (player->coins >= 200) {
                        player->coins -= 200;
                        mp5->damage +=
                            2; // Statystyka rośnie na stałe w arsenale
                    }
                }
            }
        }

        // --- KAFELEK 2: ASSAULT RIFLE (Obok, na osi X przesunięty o 270
        // pikseli) ---
        GuiGroupBox({390, 140, 250, 150}, "Karabin AK-47");
        Weapon *ak47 = player->GetWeapon(WeaponType::ASSAULT_RIFLE);

        if (ak47 != nullptr) {
            if (!ak47->isUnlocked) {
                if (GuiButton(
                        {400, 230, 230, 50},
                        TextFormat("ODBLOKUJ AK-47 [$%d]", ak47->unlockCost))) {
                    if (player->coins >= 1000) {
                        player->coins -= 1000;
                        ak47->isUnlocked = true;
                    }
                }
            } else {
                if (GuiButton({400, 170, 230, 40}, "WEZ DO REKI")) {
                    player->EquipWeapon(WeaponType::ASSAULT_RIFLE);
                }
                if (GuiButton({400, 230, 230, 40}, "ULEPSZ DMG (+3) [$350]")) {
                    if (player->coins >= 350) {
                        player->coins -= 350;
                        ak47->damage += 3;
                    }
                }
            }
        }
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
