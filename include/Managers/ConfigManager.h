#pragma once
#include "Managers/AudioManager.h"
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
};
