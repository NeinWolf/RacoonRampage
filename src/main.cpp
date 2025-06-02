#include "raylib.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>

// Add these using declarations
using std::sqrt;
using std::pow;
using std::fmax;
using std::fmin;
using std::remove_if;

// Forward declarations
class GameManager;
class Player;
class Enemy;
class Weapon;
class Ability;
class Menu;

// Enums
enum class GameState {
    MAIN_MENU,
    SETTINGS,
    CREDITS,
    SHOP,
    ARENA,
    PAUSE,
    GAME_OVER
};

enum class EnemyType {
    SLIME,
    SODA_CAN,
    PIZZA_BOX
};

// Utility Classes
struct GameTransform {
    Vector2 position;
    Vector2 scale;
    float rotation;
    
    GameTransform(Vector2 pos = {0, 0}, Vector2 sc = {1, 1}, float rot = 0) 
        : position(pos), scale(sc), rotation(rot) {}
};

// Convert world coordinates to isometric screen coordinates
Vector2 WorldToIso(Vector2 worldPos) {
    return {
        (worldPos.x - worldPos.y) * 32.0f,
        (worldPos.x + worldPos.y) * 16.0f
    };
}

// Animation system
class Animation {
private:
    std::vector<Rectangle> frames;
    int currentFrame;
    float frameTime;
    float timer;
    bool loop;

public:
    Animation(std::vector<Rectangle> frameRects, float time, bool shouldLoop = true) 
        : frames(frameRects), currentFrame(0), frameTime(time), timer(0), loop(shouldLoop) {}
    
    void Update(float deltaTime) {
        timer += deltaTime;
        if (timer >= frameTime) {
            timer = 0;
            currentFrame++;
            if (currentFrame >= static_cast<int>(frames.size())) {
                if (loop) currentFrame = 0;
                else currentFrame = static_cast<int>(frames.size()) - 1;
            }
        }
    }
    
    Rectangle GetCurrentFrame() {
        return frames[currentFrame];
    }
    
    void Reset() {
        currentFrame = 0;
        timer = 0;
    }
};

// Weapon class
class Weapon {
public:
    std::string name;
    int damage;
    float attackSpeed;
    std::string specialEffect;
    
    Weapon(std::string n, int dmg, float speed, std::string effect = "") 
        : name(n), damage(dmg), attackSpeed(speed), specialEffect(effect) {}
    
    virtual void Attack(Vector2 position, Vector2 direction) {
        // Base attack implementation
    }
};

// Ability class
class Ability {
public:
    std::string name;
    float cooldown;
    float currentCooldown;
    int damage;
    
    Ability(std::string n, float cd, int dmg) 
        : name(n), cooldown(cd), currentCooldown(0), damage(dmg) {}
    
    void Update(float deltaTime) {
        if (currentCooldown > 0) {
            currentCooldown -= deltaTime;
        }
    }
    
    bool CanUse() { return currentCooldown <= 0; }
    
    void Use() {
        if (CanUse()) {
            currentCooldown = cooldown;
            Execute();
        }
    }
    
    virtual void Execute() {}
};

// Player class
class Player {
private:
    GameTransform transform;
    Animation* currentAnimation;
    std::unordered_map<std::string, Animation*> animations;
    
public:
    int health;
    int maxHealth;
    int stamina;
    int maxStamina;
    int scraps;
    std::unique_ptr<Weapon> weapon;
    std::vector<std::unique_ptr<Ability>> abilities;
    Vector2 gridPosition;
    
    Player() : health(100), maxHealth(100), stamina(50), maxStamina(50), scraps(0), gridPosition({5, 5}) {
        // Initialize animations
        std::vector<Rectangle> idleFrames = {{0, 0, 32, 32}, {32, 0, 32, 32}};
        std::vector<Rectangle> attackFrames = {{0, 32, 32, 32}, {32, 32, 32, 32}, {64, 32, 32, 32}};
        std::vector<Rectangle> hurtFrames = {{0, 64, 32, 32}};
        
        animations["idle"] = new Animation(idleFrames, 0.5f);
        animations["attack"] = new Animation(attackFrames, 0.2f, false);
        animations["hurt"] = new Animation(hurtFrames, 0.3f, false);
        
        currentAnimation = animations["idle"];
        
        weapon = std::make_unique<Weapon>("Bottle", 10, 1.0f);
    }
    
    void Update(float deltaTime) {
        currentAnimation->Update(deltaTime);
        
        // Update abilities
        for (auto& ability : abilities) {
            ability->Update(deltaTime);
        }
        
        // Handle input
        Vector2 movement = {0, 0};
        if (IsKeyDown(KEY_W)) movement.y -= 1;
        if (IsKeyDown(KEY_S)) movement.y += 1;
        if (IsKeyDown(KEY_A)) movement.x -= 1;
        if (IsKeyDown(KEY_D)) movement.x += 1;
        
        if (movement.x != 0 || movement.y != 0) {
            gridPosition.x += movement.x * deltaTime * 3;
            gridPosition.y += movement.y * deltaTime * 3;
            
            // Keep player in bounds (adjust as needed)
            if (gridPosition.x < 0) gridPosition.x = 0;
            if (gridPosition.y < 0) gridPosition.y = 0;
            if (gridPosition.x > 20) gridPosition.x = 20;
            if (gridPosition.y > 20) gridPosition.y = 20;
        }
        
        transform.position = WorldToIso(gridPosition);
    }
    
    void Draw() {
        Vector2 screenPos = {400 + transform.position.x, 300 + transform.position.y};
        DrawRectangle(screenPos.x - 16, screenPos.y - 16, 32, 32, GREEN);
        DrawText("R", screenPos.x - 6, screenPos.y - 8, 20, WHITE);
    }
    
    void TakeDamage(int damage) {
        health -= damage;
        if (health <= 0) {
            health = 0;
        }
        currentAnimation = animations["hurt"];
        currentAnimation->Reset();
    }
    
    void Attack() {
        currentAnimation = animations["attack"];
        currentAnimation->Reset();
    }
};

// Enemy base class
class Enemy {
protected:
    GameTransform transform;
    EnemyType type;
    
public:
    int health;
    int maxHealth;
    int damage;
    float speed;
    Vector2 gridPosition;
    bool isAlive;
    
    Enemy(EnemyType t, int hp, int dmg, float spd, Vector2 pos) 
        : type(t), health(hp), maxHealth(hp), damage(dmg), speed(spd), gridPosition(pos), isAlive(true) {}
    
    virtual void Update(float deltaTime, Player* player) {
        if (!isAlive) return;
        
        // Simple AI - move towards player
        Vector2 direction = {
            player->gridPosition.x - gridPosition.x,
            player->gridPosition.y - gridPosition.y
        };
        
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) {
            direction.x /= length;
            direction.y /= length;
            
            gridPosition.x += direction.x * speed * deltaTime;
            gridPosition.y += direction.y * speed * deltaTime;
        }
        
        transform.position = WorldToIso(gridPosition);
    }
    
    virtual void Draw() = 0;
    
    void TakeDamage(int dmg) {
        health -= dmg;
        if (health <= 0) {
            isAlive = false;
        }
    }
    
    bool CheckCollision(Player* player) {
        float dist = sqrt(pow(gridPosition.x - player->gridPosition.x, 2) + 
                         pow(gridPosition.y - player->gridPosition.y, 2));
        return dist < 1.0f;
    }
};

// Specific enemy types
class Slime : public Enemy {
public:
    Slime(Vector2 pos) : Enemy(EnemyType::SLIME, 30, 10, 2.0f, pos) {}
    
    void Draw() override {
        Vector2 screenPos = {400 + transform.position.x, 300 + transform.position.y};
        DrawCircle(screenPos.x, screenPos.y, 16, GREEN);
        DrawText("S", screenPos.x - 6, screenPos.y - 8, 16, BLACK);
    }
};

class SodaCan : public Enemy {
public:
    SodaCan(Vector2 pos) : Enemy(EnemyType::SODA_CAN, 20, 15, 3.0f, pos) {}
    
    void Draw() override {
        Vector2 screenPos = {400 + transform.position.x, 300 + transform.position.y};
        DrawRectangle(screenPos.x - 8, screenPos.y - 12, 16, 24, RED);
        DrawText("C", screenPos.x - 6, screenPos.y - 8, 16, WHITE);
    }
};

class PizzaBox : public Enemy {
public:
    PizzaBox(Vector2 pos) : Enemy(EnemyType::PIZZA_BOX, 40, 8, 1.5f, pos) {}
    
    void Draw() override {
        Vector2 screenPos = {400 + transform.position.x, 300 + transform.position.y};
        DrawRectangle(screenPos.x - 16, screenPos.y - 4, 32, 8, ORANGE);
        DrawText("P", screenPos.x - 6, screenPos.y - 8, 16, BLACK);
    }
};

// Wave Manager
class WaveManager {
private:
    int currentWave;
    float waveTimer;
    float spawnTimer;
    int enemiesSpawned;
    int enemiesToSpawn;
    
public:
    WaveManager() : currentWave(1), waveTimer(0), spawnTimer(0), enemiesSpawned(0), enemiesToSpawn(5) {}
    
    void Update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
        spawnTimer += deltaTime;
        
        if (enemiesSpawned < enemiesToSpawn && spawnTimer >= 2.0f) {
            SpawnEnemy(enemies);
            spawnTimer = 0;
        }
        
        // Check if wave is complete
        bool allEnemiesDead = true;
        for (auto& enemy : enemies) {
            if (enemy->isAlive) {
                allEnemiesDead = false;
                break;
            }
        }
        
        if (allEnemiesDead && enemiesSpawned >= enemiesToSpawn) {
            NextWave();
            enemies.clear();
        }
    }
    
    void SpawnEnemy(std::vector<std::unique_ptr<Enemy>>& enemies) {
        Vector2 spawnPos = {
            static_cast<float>(GetRandomValue(0, 20)),
            static_cast<float>(GetRandomValue(0, 20))
        };
        
        int enemyType = GetRandomValue(0, 2);
        switch (enemyType) {
            case 0:
                enemies.push_back(std::make_unique<Slime>(spawnPos));
                break;
            case 1:
                enemies.push_back(std::make_unique<SodaCan>(spawnPos));
                break;
            case 2:
                enemies.push_back(std::make_unique<PizzaBox>(spawnPos));
                break;
        }
        
        enemiesSpawned++;
    }
    
    void NextWave() {
        currentWave++;
        enemiesSpawned = 0;
        enemiesToSpawn += 2; // Increase difficulty
    }
    
    int GetCurrentWave() { return currentWave; }
};

// Save System
class SaveSystem {
public:
    static void SaveHighScore(int score) {
        std::ofstream file("highscore.txt");
        if (file.is_open()) {
            file << score;
            file.close();
        }
    }
    
    static int LoadHighScore() {
        std::ifstream file("highscore.txt");
        int score = 0;
        if (file.is_open()) {
            file >> score;
            file.close();
        }
        return score;
    }
    
    static void SaveSettings(float masterVolume, float sfxVolume, float musicVolume) {
        std::ofstream file("settings.txt");
        if (file.is_open()) {
            file << masterVolume << " " << sfxVolume << " " << musicVolume;
            file.close();
        }
    }
    
    static void LoadSettings(float& masterVolume, float& sfxVolume, float& musicVolume) {
        std::ifstream file("settings.txt");
        if (file.is_open()) {
            file >> masterVolume >> sfxVolume >> musicVolume;
            file.close();
        } else {
            masterVolume = 0.5f;
            sfxVolume = 0.7f;
            musicVolume = 0.4f;
        }
    }
};

// Audio Manager
class AudioManager {
public:
    float masterVolume;
    float sfxVolume;
    float musicVolume;
    
    AudioManager() {
        SaveSystem::LoadSettings(masterVolume, sfxVolume, musicVolume);
        SetMasterVolume(masterVolume);
    }
    
    void UpdateVolumes() {
        SetMasterVolume(masterVolume);
        SaveSystem::SaveSettings(masterVolume, sfxVolume, musicVolume);
    }
};

// Menu base class
class Menu {
protected:
    GameManager* gameManager;
    
public:
    Menu(GameManager* gm) : gameManager(gm) {}
    virtual ~Menu() = default;
    virtual void Update() = 0;
    virtual void Draw() = 0;
};

// HUD class
class HUD {
public:
    void Draw(Player* player, int score, int wave) {
        // Health hearts
        for (int i = 0; i < player->maxHealth / 10; i++) {
            Color heartColor = (i < player->health / 10) ? RED : GRAY;
            DrawText("<3", 10 + i * 25, 10, 20, heartColor);
        }
        
        // Stamina dashes
        for (int i = 0; i < player->maxStamina / 5; i++) {
            Color dashColor = (i < player->stamina / 5) ? BLUE : GRAY;
            DrawText("_/ ", 10 + i * 15, 35, 20, dashColor);
        }
        
        // Score
        DrawText(TextFormat("SCORE: %04d", score), 350, 10, 20, WHITE);
        
        // Scraps
        DrawText(TextFormat("SCRAPS: %d", player->scraps), 650, 10, 20, YELLOW);
        
        // Wave
        DrawText(TextFormat("WAVE: %d", wave), 10, 550, 20, WHITE);
        
        // Abilities (bottom-left)
        for (size_t i = 0; i < player->abilities.size(); i++) {
            Rectangle abilityRect = {10.0f + static_cast<float>(i) * 60.0f, 500.0f, 50.0f, 50.0f};
            Color abilityColor = player->abilities[i]->CanUse() ? GREEN : GRAY;
            DrawRectangleRec(abilityRect, abilityColor);
            DrawRectangleLinesEx(abilityRect, 2, BLACK);
            DrawText(player->abilities[i]->name.substr(0, 1).c_str(), 
                    static_cast<int>(abilityRect.x) + 20, static_cast<int>(abilityRect.y) + 20, 20, WHITE);
        }
        
        // Weapon (bottom-right)
        Rectangle weaponRect = {730, 500, 50, 50};
        DrawRectangleRec(weaponRect, BROWN);
        DrawRectangleLinesEx(weaponRect, 2, BLACK);
        DrawText(player->weapon->name.substr(0, 1).c_str(), 
                weaponRect.x + 20, weaponRect.y + 20, 20, WHITE);
    }
};

// Main Game Manager
class GameManager {
private:
    GameState currentState;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::unique_ptr<WaveManager> waveManager;
    std::unique_ptr<AudioManager> audioManager;
    std::unique_ptr<HUD> hud;
    
    int score;
    int highScore;
    float gameTimer;
    
    // Menu variables
    int selectedMenuItem;
    bool showSettings;
    bool showCredits;
    
public:
    GameManager() : currentState(GameState::MAIN_MENU), score(0), gameTimer(0), 
                   selectedMenuItem(0), showSettings(false), showCredits(false) {
        player = std::make_unique<Player>();
        waveManager = std::make_unique<WaveManager>();
        audioManager = std::make_unique<AudioManager>();
        hud = std::make_unique<HUD>();
        highScore = SaveSystem::LoadHighScore();
    }
    
    void Update() {
        float deltaTime = GetFrameTime();
        
        switch (currentState) {
            case GameState::MAIN_MENU:
                UpdateMainMenu();
                break;
            case GameState::SETTINGS:
                UpdateSettings();
                break;
            case GameState::CREDITS:
                UpdateCredits();
                break;
            case GameState::SHOP:
                // Shop functionality not implemented yet
                currentState = GameState::ARENA;
                break;
            case GameState::ARENA:
                UpdateArena(deltaTime);
                break;
            case GameState::PAUSE:
                UpdatePause();
                break;
            case GameState::GAME_OVER:
                UpdateGameOver();
                break;
        }
    }
    
    void UpdateMainMenu() {
        if (IsKeyPressed(KEY_UP)) {
            selectedMenuItem = (selectedMenuItem - 1 + 4) % 4;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            selectedMenuItem = (selectedMenuItem + 1) % 4;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            switch (selectedMenuItem) {
                case 0: // Play
                    currentState = GameState::ARENA;
                    break;
                case 1: // Settings
                    currentState = GameState::SETTINGS;
                    break;
                case 2: // Credits
                    currentState = GameState::CREDITS;
                    break;
                case 3: // Exit
                    CloseWindow();
                    break;
            }
        }
    }
    
    void UpdateSettings() {
        if (IsKeyPressed(KEY_LEFT)) {
            audioManager->masterVolume = fmax(0.0f, audioManager->masterVolume - 0.1f);
            audioManager->UpdateVolumes();
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            audioManager->masterVolume = fmin(1.0f, audioManager->masterVolume + 0.1f);
            audioManager->UpdateVolumes();
        }
        if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_ENTER)) {
            currentState = GameState::MAIN_MENU;
        }
    }
    
    void UpdateCredits() {
        if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_ENTER)) {
            currentState = GameState::MAIN_MENU;
        }
    }
    
    void UpdateArena(float deltaTime) {
        if (IsKeyPressed(KEY_BACKSPACE)) {
            currentState = GameState::PAUSE;
            return;
        }
        
        gameTimer += deltaTime;
        
        // Update player
        player->Update(deltaTime);
        
        // Update wave manager
        waveManager->Update(deltaTime, enemies);
        
        // Update enemies
        for (auto& enemy : enemies) {
            enemy->Update(deltaTime, player.get());
            
            // Check collision with player
            if (enemy->isAlive && enemy->CheckCollision(player.get())) {
                player->TakeDamage(enemy->damage);
                enemy->TakeDamage(100); // Kill enemy on hit for now
                score += 10;
            }
        }
        
        // Check if player is dead
        if (player->health <= 0) {
            if (score > highScore) {
                highScore = score;
                SaveSystem::SaveHighScore(highScore);
            }
            currentState = GameState::GAME_OVER;
        }
        
        // Remove dead enemies
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const std::unique_ptr<Enemy>& enemy) { return !enemy->isAlive; }),
            enemies.end()
        );
    }
    
    void UpdatePause() {
        if (IsKeyPressed(KEY_UP)) {
            selectedMenuItem = (selectedMenuItem - 1 + 3) % 3;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            selectedMenuItem = (selectedMenuItem + 1) % 3;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            switch (selectedMenuItem) {
                case 0: // Resume
                    currentState = GameState::ARENA;
                    break;
                case 1: // Settings
                    currentState = GameState::SETTINGS;
                    break;
                case 2: // Exit
                    ResetGame();
                    currentState = GameState::MAIN_MENU;
                    break;
            }
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            currentState = GameState::ARENA;
        }
    }
    
    void UpdateGameOver() {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            ResetGame();
            currentState = GameState::MAIN_MENU;
        }
    }
    
    void ResetGame() {
        player = std::make_unique<Player>();
        enemies.clear();
        waveManager = std::make_unique<WaveManager>();
        score = 0;
        gameTimer = 0;
        selectedMenuItem = 0;
    }
    
    void Draw() {
        BeginDrawing();
        ClearBackground(BLACK);
        
        switch (currentState) {
            case GameState::MAIN_MENU:
                DrawMainMenu();
                break;
            case GameState::SETTINGS:
                DrawSettings();
                break;
            case GameState::CREDITS:
                DrawCredits();
                break;
            case GameState::SHOP:
                // Shop drawing not implemented yet
                DrawText("SHOP - Coming Soon!", 300, 300, 30, WHITE);
                break;
            case GameState::ARENA:
                DrawArena();
                break;
            case GameState::PAUSE:
                DrawPause();
                break;
            case GameState::GAME_OVER:
                DrawGameOver();
                break;
        }
        
        EndDrawing();
    }
    
    void DrawMainMenu() {
        DrawText("RACCOON RAMPAGE", 250, 100, 40, GREEN);
        
        const char* menuItems[] = {"PLAY", "SETTINGS", "CREDITS", "EXIT"};
        for (int i = 0; i < 4; i++) {
            Color textColor = (i == selectedMenuItem) ? YELLOW : WHITE;
            DrawText(menuItems[i], 350, 200 + i * 50, 30, textColor);
        }
        
        DrawText(TextFormat("HIGH SCORE: %d", highScore), 300, 450, 20, WHITE);
    }
    
    void DrawSettings() {
        DrawText("SETTINGS", 350, 100, 40, WHITE);
        
        DrawText("VOLUME", 200, 200, 30, WHITE);
        DrawRectangle(300, 210, 200, 20, GRAY);
        DrawRectangle(300, 210, audioManager->masterVolume * 200, 20, GREEN);
        
        DrawText("SFX", 200, 250, 30, WHITE);
        DrawRectangle(300, 260, 200, 20, GRAY);
        DrawRectangle(300, 260, audioManager->sfxVolume * 200, 20, BLUE);
        
        DrawText("MUSIC", 200, 300, 30, WHITE);
        DrawRectangle(300, 310, 200, 20, GRAY);
        DrawRectangle(300, 310, audioManager->musicVolume * 200, 20, RED);
        
        DrawText("Use LEFT/RIGHT arrows to adjust", 250, 400, 20, WHITE);
        DrawText("Press BACKSPACE to go back", 300, 450, 20, WHITE);
    }
    
    void DrawCredits() {
        DrawText("CREDITS", 350, 100, 40, WHITE);
        DrawText("Game by: Mikolaj Kimak", 250, 200, 30, WHITE);
        DrawText("Made with Raylib", 280, 250, 30, WHITE);
        DrawText("Press BACKSPACE to go back", 300, 450, 20, WHITE);
    }
    
    void DrawArena() {
        // Draw arena background
        for (int x = 0; x < 25; x++) {
            for (int y = 0; y < 25; y++) {
                Vector2 isoPos = WorldToIso({static_cast<float>(x), static_cast<float>(y)});
                Vector2 screenPos = {400 + isoPos.x, 300 + isoPos.y};
                
                if (x % 2 == y % 2) {
                    DrawRectangle(screenPos.x - 16, screenPos.y - 8, 32, 16, DARKGRAY);
                } else {
                    DrawRectangle(screenPos.x - 16, screenPos.y - 8, 32, 16, GRAY);
                }
            }
        }
        
        // Draw enemies
        for (auto& enemy : enemies) {
            enemy->Draw();
        }
        
        // Draw player
        player->Draw();
        
        // Draw HUD
        hud->Draw(player.get(), score, waveManager->GetCurrentWave());
    }
    
    void DrawPause() {
        // Draw arena in background (dimmed)
        DrawArena();
        DrawRectangle(0, 0, 800, 600, Fade(BLACK, 0.7f));
        
        DrawText("PAUSED", 350, 200, 40, WHITE);
        
        const char* pauseItems[] = {"RESUME", "SETTINGS", "EXIT TO MENU"};
        for (int i = 0; i < 3; i++) {
            Color textColor = (i == selectedMenuItem) ? YELLOW : WHITE;
            DrawText(pauseItems[i], 300, 300 + i * 40, 25, textColor);
        }
    }
    
    void DrawGameOver() {
        DrawText("GAME OVER", 300, 200, 40, RED);
        DrawText(TextFormat("FINAL SCORE: %d", score), 300, 280, 30, WHITE);
        
        if (score > highScore) {
            DrawText("NEW HIGH SCORE!", 280, 320, 30, YELLOW);
        }
        
        DrawText("Press ENTER to continue", 250, 400, 25, WHITE);
    }
    
    bool ShouldClose() {
        return WindowShouldClose();
    }
};

// Main function
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