#pragma once
#include "GameState.h"
#include "../Entities/Player.h"
#include "../Entities/Enemy.h"
#include "../Systems/WaveManager.h"
#include "../Systems/AudioManager.h"
#include "../Systems/HUD.h"
#include "../UI/Menu.h"
#include <vector>
#include <memory>

class GameManager {
private:
    GameState currentState;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::unique_ptr<WaveManager> waveManager;
    std::unique_ptr<AudioManager> audioManager;
    std::unique_ptr<HUD> hud;
    
    // Menu system
    std::unique_ptr<Menu> currentMenu;
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
};