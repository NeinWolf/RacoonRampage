#pragma once
#include <string>
#include <fstream>

class SaveSystem {
public:
    static void SaveHighScore(int score, int scraps);
    static int LoadHighScore(int& scraps);
    static void SaveSettings(float masterVolume, float sfxVolume, float musicVolume);
    static void LoadSettings(float& masterVolume, float& sfxVolume, float& musicVolume);
    static void SaveScraps(int scraps);
    static int LoadScraps();

    static void SaveWeapon(const std::string& weaponName);
    static std::string LoadWeapon();
};