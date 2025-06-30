#include "PauseMenu.h"
#include "raylib.h"
#include "GameState.h"

PauseMenu::PauseMenu(GameManager* gm) : Menu(gm), selectedMenuItem(0) {}

void PauseMenu::Update() {
    if (IsKeyPressed(KEY_UP)) {
        selectedMenuItem = (selectedMenuItem - 1 + 3) % 3;
    }
    if (IsKeyPressed(KEY_DOWN)) {
        selectedMenuItem = (selectedMenuItem + 1) % 3;
    }
    if (IsKeyPressed(KEY_ENTER)) {
        switch (selectedMenuItem) {
            case 0: // Resume
                gameManager->SetGameState(GameState::ARENA);
                break;
            case 1: // Settings
                gameManager->SetGameState(GameState::SETTINGS);
                break;
            case 2: // Exit
                gameManager->ResetGame();
                gameManager->SetGameState(GameState::MAIN_MENU);
                break;
        }
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        gameManager->SetGameState(GameState::ARENA);
    }
}

void PauseMenu::Draw() {
    DrawRectangle(0, 0, 800, 600, Fade(BLACK, 0.7f));
    DrawText("PAUSED", 350, 200, 40, WHITE);
    
    const char* pauseItems[] = {"RESUME", "SETTINGS", "EXIT TO MENU"};
    for (int i = 0; i < 3; i++) {
        Color textColor = (i == selectedMenuItem) ? YELLOW : WHITE;
        DrawText(pauseItems[i], 300, 300 + i * 40, 25, textColor);
    }
}