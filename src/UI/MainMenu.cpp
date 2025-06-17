#include "MainMenu.h"
#include "raylib.h"

MainMenu::MainMenu(GameManager* gm) : Menu(gm), selectedMenuItem(0) {}

void MainMenu::Update() {
    if (IsKeyPressed(KEY_UP)) {
        selectedMenuItem = (selectedMenuItem - 1 + 4) % 4;
    }
    if (IsKeyPressed(KEY_DOWN)) {
        selectedMenuItem = (selectedMenuItem + 1) % 4;
    }
    if (IsKeyPressed(KEY_ENTER)) {
        switch (selectedMenuItem) {
            case 0: // Play
                gameManager->SetGameState(GameState::ARENA);
                break;
            case 1: // Settings
                gameManager->SetGameState(GameState::SETTINGS);
                break;
            case 2: // Credits
                gameManager->SetGameState(GameState::CREDITS);
                break;
            case 3: // Exit
                gameManager->SetShouldClose(true);
                break;
        }
    }
}

void MainMenu::Draw() {
    DrawText("RACCOON RAMPAGE", 250, 100, 40, GREEN);
    
    const char* menuItems[] = {"PLAY", "SETTINGS", "CREDITS", "EXIT"};
    for (int i = 0; i < 4; i++) {
        Color textColor = (i == selectedMenuItem) ? YELLOW : WHITE;
        DrawText(menuItems[i], 350, 200 + i * 50, 30, textColor);
    }
    
    DrawText(TextFormat("HIGH SCORE: %d", gameManager->GetHighScore()), 300, 450, 20, WHITE);
}