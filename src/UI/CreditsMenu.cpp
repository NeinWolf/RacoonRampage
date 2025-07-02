#include "CreditsMenu.h"
#include "raylib.h"

CreditsMenu::CreditsMenu(GameManager* gm) : Menu(gm) {}

void CreditsMenu::Update() {
    if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_ENTER)) {
        gameManager->SetGameState(GameState::MAIN_MENU);
    }
}

void CreditsMenu::Draw() {
    DrawText("CREDITS", 350, 100, 40, WHITE);
    DrawText("Developer: Mikołaj Kimak", 200, 200, 20, WHITE);
    DrawText("Press BACKSPACE to go back", 220, 400, 20, WHITE);
}
