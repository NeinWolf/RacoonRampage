#include "MainMenu.h"
#include "raylib.h"

MainMenu::MainMenu(GameManager* gm) : Menu(gm), selectedMenuItem(0) {}

void MainMenu::Update() {
    const int menuCount = 5;
    if (IsKeyPressed(KEY_UP)) {
        selectedMenuItem = (selectedMenuItem - 1 + menuCount) % menuCount;
    }
    if (IsKeyPressed(KEY_DOWN)) {
        selectedMenuItem = (selectedMenuItem + 1) % menuCount;
    }
    if (IsKeyPressed(KEY_ENTER)) {
        switch (selectedMenuItem) {
            case 0: // Play
                gameManager->SetGameState(GameState::ARENA);
                break;
            case 1: // Shop
                gameManager->SetGameState(GameState::SHOP);
                break;
            case 2: // Settings
                gameManager->SetGameState(GameState::SETTINGS);
                break;
            case 3: // Credits
                gameManager->SetGameState(GameState::CREDITS);
                break;
            case 4: // Exit
                gameManager->SetShouldClose(true);
                break;
        }
    }
}

void MainMenu::Draw() {
    DrawText("RACCOON RAMPAGE", 250, 100, 40, GREEN);
    
    const char* menuItems[] = {"PLAY", "SHOP", "SETTINGS", "CREDITS", "EXIT"};
    const int menuCount = 5;
    for (int i = 0; i < menuCount; i++) {
        Color textColor = (i == selectedMenuItem) ? YELLOW : WHITE;
        DrawText(menuItems[i], 350, 200 + i * 50, 30, textColor);
    }

    DrawText(TextFormat("HIGH SCORE: %d", gameManager->GetHighScore()), 300, 450, 20, WHITE);
    DrawText(TextFormat("SCRAPS: %d", gameManager->GetPlayer()->GetScraps()), 300, 480, 20, YELLOW);
}