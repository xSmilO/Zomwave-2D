#pragma once
#include "Managers/AudioManager.h"
#include "Managers/GameBalance.h"
#include "raylib.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ConfigManager {

  public:
    static void LoadSettings(AudioManager &audioManager, int &currentFpsIndex,
                             const std::vector<int> &fpsOptions) {
        std::ifstream file("settings.json");


        if (!file.is_open())
            return;

        json j;
        file >> j;

        audioManager.SetMusicVolume(j.value("musicVolume", 1.0f));
        audioManager.SetSFXVolume(j.value("sfxVolume", 1.0f));

        currentFpsIndex = j.value("fpsIndex", 1);

        if (currentFpsIndex >= 0 && currentFpsIndex < fpsOptions.size()) {
            SetTargetFPS(fpsOptions[currentFpsIndex]);
        }
    }

    static void SaveSettings(AudioManager &audioManager, int currentFpsIndex) {
        json j;
        j["musicVolume"] = audioManager.GetMusicVolume();
        j["sfxVolume"] = audioManager.GetSFXVolume();
        j["fpsIndex"] = currentFpsIndex;

        std::ofstream file("settings.json");
        file << j.dump(4);
    }

    static GameBalance LoadBalance(const std::string& filepath = "balance.json") {
        GameBalance balance;
        std::ifstream file(filepath);

        if(file.is_open()) {
            try {
                json j;
                file >> j;
                balance = j.get<GameBalance>();
            } catch(const std::exception& e) {
                printf("ERROR WHILE LOADING A CONFIG %s\n", e.what());
            }
        } else {
            printf("Couldn't find a json");
        }

        return balance;
    }
};
