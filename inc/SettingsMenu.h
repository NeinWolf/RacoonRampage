#pragma once
#include "Menu.h"
#include "AudioManager.h"

class SettingsMenu : public Menu {
private:
    AudioManager* audioManager;
    
public:
    SettingsMenu(GameManager* gm, AudioManager* am);
    void Update() override;
    void Draw() override;
};