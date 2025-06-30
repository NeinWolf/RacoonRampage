#include "SettingsMenu.h"
#include "raylib.h"
#include <cmath>

SettingsMenu::SettingsMenu(GameManager* gm, AudioManager* am) 
    : Menu(gm), audioManager(am) {}

void SettingsMenu::Update() {
    if (IsKeyPressed(KEY_LEFT)) {
        audioManager->SetMasterVolume(std::fmax(0.0f, audioManager->GetMasterVolume() - 0.1f));
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        audioManager->SetMasterVolume(std::fmin(1.0f, audioManager->GetMasterVolume() + 0.1f));
    }
    if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_ENTER)) {
        gameManager->SetGameState(GameState::MAIN_MENU);
    }
}

void SettingsMenu::Draw() {
    DrawText("SETTINGS", 350, 100, 40, WHITE);
    
    DrawText("VOLUME", 200, 200, 30, WHITE);
    DrawRectangle(300, 210, 200, 20, GRAY);
    DrawRectangle(300, 210, audioManager->GetMasterVolume() * 200, 20, GREEN);
    
    DrawText("Use LEFT/RIGHT arrows to adjust", 250, 400, 20, WHITE);
    DrawText("Press BACKSPACE to go back", 300, 450, 20, WHITE);
}