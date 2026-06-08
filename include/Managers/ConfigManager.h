#pragma once
#include "Managers/AudioManager.h"
#include "Managers/GameBalance.h"
#include "raylib.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * @class ConfigManager
 * @brief Static utility class for loading and saving game configuration.
 *
 * Handles persistent settings (audio volume, FPS limit) via a JSON file
 * and loads the game balance configuration from a separate JSON file.
 */
class ConfigManager {

  public:
    /**
     * @brief Loads audio and FPS settings from "settings.json".
     * @param audioManager    Reference to the audio manager to apply volume settings.
     * @param currentFpsIndex Output parameter for the FPS limit index.
     * @param fpsOptions      List of available FPS options.
     */
    static void LoadSettings(AudioManager &audioManager, int &currentFpsIndex, const std::vector<int> &fpsOptions) {
        std::ifstream file("settings.json");

        if(!file.is_open())
            return;

        json j;
        file >> j;

        audioManager.SetMusicVolume(j.value("musicVolume", 1.0f));
        audioManager.SetSFXVolume(j.value("sfxVolume", 1.0f));

        currentFpsIndex = j.value("fpsIndex", 1);

        if(currentFpsIndex >= 0 && currentFpsIndex < fpsOptions.size()) {
            SetTargetFPS(fpsOptions[currentFpsIndex]);
        }
    }

    /**
     * @brief Saves audio and FPS settings to "settings.json".
     * @param audioManager    Reference to the audio manager to read volume from.
     * @param currentFpsIndex Current FPS limit index.
     */
    static void SaveSettings(AudioManager &audioManager, int currentFpsIndex) {
        json j;
        j["musicVolume"] = audioManager.GetMusicVolume();
        j["sfxVolume"] = audioManager.GetSFXVolume();
        j["fpsIndex"] = currentFpsIndex;

        std::ofstream file("settings.json");
        file << j.dump(4);
    }

    /**
     * @brief Loads the game balance configuration from a JSON file.
     * @param filepath Path to the balance JSON file (default: "balance.json").
     * @return A populated GameBalance struct.
     */
    static GameBalance LoadBalance(const std::string &filepath = "balance.json") {
        GameBalance balance;
        std::ifstream file(filepath);

        if(file.is_open()) {
            try {
                json j;
                file >> j;
                balance = j.get<GameBalance>();
            } catch(const std::exception &e) {
                printf("ERROR WHILE LOADING A CONFIG %s\n", e.what());
            }
        } else {
            printf("Couldn't find a json");
        }

        return balance;
    }
};
