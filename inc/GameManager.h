#pragma once

#include "GameState.h"
#include "Player.h"
#include "Enemy.h"
#include "WaveManager.h"
#include "AudioManager.h"
#include "HUD.h"

#include <vector>
#include <memory>
#include <unordered_map>

// Forward declaration of Menu to avoid circular dependency
class Menu;

class GameManager {
private:
    GameState currentState;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::unique_ptr<WaveManager> waveManager;
    std::unique_ptr<AudioManager> audioManager;
    std::unique_ptr<HUD> hud;
    
    // Menu system
    Menu* currentMenu;   // raw pointer since menus hold ownership
    std::unordered_map<GameState, std::unique_ptr<Menu>> menus;
    
    int score;
    int highScore;
    float gameTimer;
    bool shouldClose;

public:
    GameManager();
    ~GameManager();
    
    void Update();
    void Draw();
    bool ShouldClose() const;
    void SetShouldClose(bool value) { shouldClose = value; }

    // State management
    void SetGameState(GameState state);
    GameState GetGameState() const;
    
    // Game management
    void ResetGame();
    void AddScore(int amount);
    
    // Getters
    int GetScore() const { return score; }
    int GetHighScore() const { return highScore; }
    Player* GetPlayer() { return player.get(); }
    
private:
    void InitializeMenus();
    void UpdateArena(float deltaTime);
    void CleanupEnemies();
    void ResolveEnemyCollisions();
    void ResolvePlayerEnemyCollisions();
};
