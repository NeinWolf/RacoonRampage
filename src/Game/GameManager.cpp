#include "GameManager.h"
#include "MainMenu.h"
#include "SettingsMenu.h"
#include "PauseMenu.h"
#include "Utils.h"
#include "raylib.h"
#include <ranges>
#include <cmath>

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
    player->SetScraps(SaveSystem::LoadScraps());
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
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        Vector2 iso = { mouse.x - 400, mouse.y - 300 };
        Vector2 world = Utils::IsoToWorld(iso);

        Vector2 dir = {
            world.x - player->GetGridPosition().x,
            world.y - player->GetGridPosition().y
        };

        float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
        if (len > 0) {
            dir.x /= len;
            dir.y /= len;
        }

        player->Attack(dir);

        Vector2 atkPos = player->GetGridPosition();
        atkPos.x += dir.x;
        atkPos.y += dir.y;

        for (auto& enemy : enemies) {
            if (!enemy->IsAlive()) continue;
            Vector2 epos = enemy->GetGridPosition();
            float dx = epos.x - atkPos.x;
            float dy = epos.y - atkPos.y;
            if (dx * dx + dy * dy <= 1.0f) {
                bool wasAlive = enemy->IsAlive();
                enemy->TakeDamage(player->GetWeapon()->GetDamage());
                if (wasAlive && !enemy->IsAlive()) {
                    AddScore(10);
                }
            }
        }
    }
    player->Update(deltaTime);
    waveManager->Update(deltaTime, enemies);
    
    for (auto& enemy : enemies) {
        int prevHealth = enemy->GetHealth();
        enemy->Update(deltaTime, player.get());

        if (prevHealth > 0 && !enemy->IsAlive()) {
            AddScore(10);
        }
    }

    ResolveEnemyCollisions();
    ResolvePlayerEnemyCollisions();
    
    if (player->GetHealth() <= 0) {
        if (score > highScore) {
            highScore = score;
            SaveSystem::SaveHighScore(highScore);
        }
        SaveSystem::SaveScraps(player->GetScraps());
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

void GameManager::ResolveEnemyCollisions() {
    const float minDist = 0.8f;
    for (size_t i = 0; i < enemies.size(); ++i) {
        for (size_t j = i + 1; j < enemies.size(); ++j) {
            if (!enemies[i]->IsAlive() || !enemies[j]->IsAlive()) continue;
            Vector2 posA = enemies[i]->GetGridPosition();
            Vector2 posB = enemies[j]->GetGridPosition();
            float dx = posB.x - posA.x;
            float dy = posB.y - posA.y;
            float distSq = dx * dx + dy * dy;
            if (distSq < minDist * minDist && distSq > 0.0001f) {
                float dist = sqrtf(distSq);
                float overlap = (minDist - dist) / 2.0f;
                dx /= dist;
                dy /= dist;
                posA.x -= dx * overlap;
                posA.y -= dy * overlap;
                posB.x += dx * overlap;
                posB.y += dy * overlap;
                posA.x = Utils::Clamp(posA.x, 0.0f, 20.0f);
                posA.y = Utils::Clamp(posA.y, 0.0f, 20.0f);
                posB.x = Utils::Clamp(posB.x, 0.0f, 20.0f);
                posB.y = Utils::Clamp(posB.y, 0.0f, 20.0f);
                enemies[i]->SetGridPosition(posA);
                enemies[j]->SetGridPosition(posB);
            }
        }
    }
}

void GameManager::ResolvePlayerEnemyCollisions() {
    const float minDist = 0.8f;
    Vector2 pPos = player->GetGridPosition();
    for (auto& enemy : enemies) {
        if (!enemy->IsAlive()) continue;
        Vector2 ePos = enemy->GetGridPosition();
        float dx = ePos.x - pPos.x;
        float dy = ePos.y - pPos.y;
        float distSq = dx * dx + dy * dy;
        if (distSq < minDist * minDist && distSq > 0.0001f) {
            float dist = sqrtf(distSq);
            float overlap = (minDist - dist);
            dx /= dist;
            dy /= dist;
            pPos.x -= dx * overlap * 0.5f;
            pPos.y -= dy * overlap * 0.5f;
            ePos.x += dx * overlap * 0.5f;
            ePos.y += dy * overlap * 0.5f;
            pPos.x = Utils::Clamp(pPos.x, 0.0f, 20.0f);
            pPos.y = Utils::Clamp(pPos.y, 0.0f, 20.0f);
            ePos.x = Utils::Clamp(ePos.x, 0.0f, 20.0f);
            ePos.y = Utils::Clamp(ePos.y, 0.0f, 20.0f);
            player->SetGridPosition(pPos);
            enemy->SetGridPosition(ePos);
        }
    }
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
    player->SetScraps(SaveSystem::LoadScraps());
    enemies.clear();
    waveManager = std::make_unique<WaveManager>();
    score = 0;
    gameTimer = 0;
}

void GameManager::AddScore(int amount) {
    score += amount;
    player->AddScraps(amount / 10); // Convert some score to scraps
    SaveSystem::SaveScraps(player->GetScraps());
}

bool GameManager::ShouldClose() const {
    return shouldClose;
}
