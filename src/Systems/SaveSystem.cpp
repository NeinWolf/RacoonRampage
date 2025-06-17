#include "SaveSystem.h"

void SaveSystem::SaveHighScore(int score) {
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        file << score;
        file.close();
    }
}

int SaveSystem::LoadHighScore() {
    std::ifstream file("highscore.txt");
    int score = 0;
    if (file.is_open()) {
        file >> score;
        file.close();
    }
    return score;
}

void SaveSystem::SaveSettings(float masterVolume, float sfxVolume, float musicVolume) {
    std::ofstream file("settings.txt");
    if (file.is_open()) {
        file << masterVolume << " " << sfxVolume << " " << musicVolume;
        file.close();
    }
}

void SaveSystem::LoadSettings(float& masterVolume, float& sfxVolume, float& musicVolume) {
    std::ifstream file("settings.txt");
    if (file.is_open()) {
        file >> masterVolume >> sfxVolume >> musicVolume;
        file.close();
    } else {
        masterVolume = 0.5f;
        sfxVolume = 0.7f;
        musicVolume = 0.4f;
    }
}