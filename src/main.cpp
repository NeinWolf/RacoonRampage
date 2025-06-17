#include "Game/GameManager.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "Raccoon Rampage");
    InitAudioDevice();
    SetTargetFPS(60);
    
    GameManager gameManager;
    
    while (!gameManager.ShouldClose()) {
        gameManager.Update();
        gameManager.Draw();
    }
    
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}