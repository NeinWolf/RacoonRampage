#include "SaveSystem.h"
#include <filesystem>
#include <thread>

namespace fs = std::filesystem;

void SaveSystem::SaveHighScore(int score) {
    std::thread([score]() {
        fs::path dir = fs::current_path() / "saves";
        fs::create_directories(dir);
        std::ofstream file(dir / "highscore.txt");
        if (file.is_open()) {
            file << score;
        }
    }).detach();
}

int SaveSystem::LoadHighScore() {
    fs::path path = fs::current_path() / "saves" / "highscore.txt";
    std::ifstream file(path);
    int score = 0;
    if (file.is_open()) {
        file >> score;
    }
    return score;
}

void SaveSystem::SaveSettings(float masterVolume, float sfxVolume, float musicVolume) {
    std::thread([masterVolume, sfxVolume, musicVolume]() {
        fs::path dir = fs::current_path() / "saves";
        fs::create_directories(dir);
        std::ofstream file(dir / "settings.txt");
        if (file.is_open()) {
            file << masterVolume << ' ' << sfxVolume << ' ' << musicVolume;
        }
    }).detach();
}

void SaveSystem::LoadSettings(float& masterVolume, float& sfxVolume, float& musicVolume) {
    fs::path path = fs::current_path() / "saves" / "settings.txt";
    std::ifstream file(path);
    if (file.is_open()) {
        file >> masterVolume >> sfxVolume >> musicVolume;
    } else {
        masterVolume = 0.5f;
        sfxVolume = 0.7f;
        musicVolume = 0.4f;
    }
}