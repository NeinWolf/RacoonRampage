#include "GameManager.h"
#include "MainMenu.h"
#include "SettingsMenu.h"
#include "PauseMenu.h"
#include "Utils.h"
#include "raylib.h"
#include <ranges>

GameManager::GameManager() : 
    currentState(GameState::MAIN_MENU),
    score(0),
    highScore(0),
    gameTimer(0),
    shouldClose(false),
    currentMenu(nullptr) {   // Initialize pointer to nullptr
    
    player = std::make_unique<Player>();
    waveManager = std::make_unique<WaveManager>();
    audioManager = std::make_unique<AudioManager>();
    hud = std::make_unique<HUD>();
    
    highScore = SaveSystem::LoadHighScore();
    InitializeMenus();
}

GameManager::~GameManager() {
    // Cleanup handled by smart pointers
}

void GameManager::InitializeMenus() {
    menus[GameState::MAIN_MENU] = std::make_unique<MainMenu>(this);
    menus[GameState::SETTINGS] = std::make_unique<SettingsMenu>(this, audioManager.get());
    menus[GameState::PAUSE] = std::make_unique<PauseMenu>(this);
    
    currentMenu = menus[GameState::MAIN_MENU].get();  // Use raw pointer instead of moving unique_ptr
}

void GameManager::Update() {
    float deltaTime = GetFrameTime();
    
    switch (currentState) {
        case GameState::MAIN_MENU:
        case GameState::SETTINGS:
        case GameState::PAUSE:
            if(currentMenu)
                currentMenu->Update();
            break;
            
        case GameState::ARENA:
            UpdateArena(deltaTime);
            break;
            
        case GameState::GAME_OVER:
            if (IsKeyPressed(KEY_ENTER)) {
                ResetGame();
                SetGameState(GameState::MAIN_MENU);
            }
            break;
            
        default: break;
    }
}

void GameManager::UpdateArena(float deltaTime) {
    if (IsKeyPressed(KEY_BACKSPACE)) {
        SetGameState(GameState::PAUSE);
        return;
    }
    
    gameTimer += deltaTime;
    if (IsKeyPressed(KEY_SPACE)) {
        player->Attack();

        Vector2 atkPos = player->GetGridPosition();
        Vector2 dir = player->GetLastMoveDir();
        atkPos.x += dir.x;
        atkPos.y += dir.y;

        for (auto& enemy : enemies) {
            if (!enemy->IsAlive()) continue;
            Vector2 epos = enemy->GetGridPosition();
            float dx = epos.x - atkPos.x;
            float dy = epos.y - atkPos.y;
            if (dx * dx + dy * dy <= 1.0f) {
                enemy->TakeDamage(player->GetWeapon()->GetDamage());
            }
        }
    }
    player->Update(deltaTime);
    waveManager->Update(deltaTime, enemies);
    
    for (auto& enemy : enemies) {
        enemy->Update(deltaTime, player.get());
        
        if (enemy->IsAlive() && enemy->CheckCollision(player.get())) {
            player->TakeDamage(enemy->GetDamage());
            enemy->TakeDamage(100); // Instant kill for now
            AddScore(10);
        }
    }
    
    if (player->GetHealth() <= 0) {
        if (score > highScore) {
            highScore = score;
            SaveSystem::SaveHighScore(highScore);
        }
        SetGameState(GameState::GAME_OVER);
    }
    
    CleanupEnemies();
}

void GameManager::CleanupEnemies() {
    auto subrange = std::ranges::remove_if(enemies,
        [](const std::unique_ptr<Enemy>& enemy) {
            return !enemy->IsAlive();
        });
    enemies.erase(subrange.begin(), subrange.end());
}

void GameManager::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);
    
    switch (currentState) {
        case GameState::MAIN_MENU:
        case GameState::SETTINGS:
        case GameState::PAUSE:
            if(currentMenu)
                currentMenu->Draw();
            break;
            
        case GameState::ARENA:
            // Draw arena background
            for (int x = 0; x < 25; x++) {
                for (int y = 0; y < 25; y++) {
                    Vector2 isoPos = Utils::WorldToIso({static_cast<float>(x), static_cast<float>(y)});
                    Vector2 screenPos = {400 + isoPos.x, 300 + isoPos.y};
                    
                    Color tileColor = (x % 2 == y % 2) ? DARKGRAY : GRAY;
                    DrawRectangle(screenPos.x - 16, screenPos.y - 8, 32, 16, tileColor);
                }
            }
            
            // Draw entities
            for (auto& enemy : enemies) {
                enemy->Draw();
            }
            player->Draw();
            hud->Draw(player.get(), score, waveManager->GetCurrentWave());
            break;
            
        case GameState::GAME_OVER:
            DrawText("GAME OVER", 300, 200, 40, RED);
            DrawText(TextFormat("FINAL SCORE: %d", score), 300, 280, 30, WHITE);
            if (score > highScore) {
                DrawText("NEW HIGH SCORE!", 280, 320, 30, YELLOW);
            }
            DrawText("Press ENTER to continue", 250, 400, 25, WHITE);
            break;
            
        default: break;
    }
    
    EndDrawing();
}

void GameManager::SetGameState(GameState state) {
    currentState = state;
    if (menus.find(state) != menus.end()) {
        currentMenu = menus[state].get();  // Use raw pointer here too
    }
}

void GameManager::ResetGame() {
    player = std::make_unique<Player>();
    enemies.clear();
    waveManager = std::make_unique<WaveManager>();
    score = 0;
    gameTimer = 0;
}

void GameManager::AddScore(int amount) {
    score += amount;
    player->AddScraps(amount / 10); // Convert some score to scraps
}

bool GameManager::ShouldClose() const {
    return shouldClose;
}
