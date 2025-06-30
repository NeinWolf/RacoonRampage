#include "SaveSystem.h"
#include <filesystem>
#include <thread>

namespace fs = std::filesystem;

void SaveSystem::SaveHighScore(int score, int scraps) {
    std::thread([score, scraps]() {
        fs::path dir = fs::current_path() / "saves";
        fs::create_directories(dir);
        std::ofstream file(dir / "highscore.txt");
        if (file.is_open()) {
            file << score << ' ' << scraps;
        }
    }).detach();
}

int SaveSystem::LoadHighScore(int& scraps) {
    fs::path path = fs::current_path() / "saves" / "highscore.txt";
    std::ifstream file(path);
    int score = 0;
    if (file.is_open()) {
        file >> score >> scraps;
    } else {
        scraps = 0;
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

void SaveSystem::SaveScraps(int scraps) {
    int currentScraps = 0;
    int currentScore = LoadHighScore(currentScraps);
    SaveHighScore(currentScore, scraps);
}

int SaveSystem::LoadScraps() {
    int scraps = 0;
    LoadHighScore(scraps);
    return scraps;
}

void SaveSystem::SaveWeapon(const std::string& weaponName) {
    std::thread([weaponName]() {
        fs::path dir = fs::current_path() / "saves";
        fs::create_directories(dir);
        std::ofstream file(dir / "weapon.txt");
        if (file.is_open()) {
            file << weaponName;
        }
    }).detach();
}

std::string SaveSystem::LoadWeapon() {
    fs::path path = fs::current_path() / "saves" / "weapon.txt";
    std::ifstream file(path);
    std::string weaponName;
    if (file.is_open()) {
        file >> weaponName;
    } else {
        weaponName = "Bottle";
    }
    return weaponName;
}