#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include <thread>

class SaveSystem {
public:
    static void SaveHighScore(int score);
    static int LoadHighScore();
    static void SaveSettings(float masterVolume, float sfxVolume, float musicVolume);
    static void LoadSettings(float& masterVolume, float& sfxVolume, float& musicVolume);
    static void SaveScraps(int scraps);
    static int LoadScraps();

    static void SaveWeapon(const std::string& weaponName);
    static std::string LoadWeapon();
};