#include "WaveManager.h"
#include "Slime.h"
#include "SodaCan.h"
#include "PizzaBox.h"
#include "raylib.h"
#include <ranges>

WaveManager::WaveManager() 
    : currentWave(1), waveTimer(0), spawnTimer(0), enemiesSpawned(0), enemiesToSpawn(5) {}

void WaveManager::Update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
    spawnTimer += deltaTime;
    
    if (enemiesSpawned < enemiesToSpawn && spawnTimer >= 2.0f) {
        SpawnEnemy(enemies);
        spawnTimer = 0;
    }
    
    // Check if wave is complete
    bool allEnemiesDead = std::ranges::all_of(enemies,
        [](const std::unique_ptr<Enemy>& enemy) {
            return !enemy->IsAlive();
        });
    
    if (allEnemiesDead && enemiesSpawned >= enemiesToSpawn) {
        NextWave();
        enemies.clear();
    }
}

void WaveManager::SpawnEnemy(std::vector<std::unique_ptr<Enemy>>& enemies) {
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

void WaveManager::NextWave() {
    currentWave++;
    enemiesSpawned = 0;
    enemiesToSpawn += 2;
}

int WaveManager::GetCurrentWave() const {
    return currentWave;
}