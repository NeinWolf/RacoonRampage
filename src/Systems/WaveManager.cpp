#include "WaveManager.h"
#include "Slime.h"
#include "SodaCan.h"
#include "PizzaBox.h"
#include "raylib.h"
#include <algorithm>

WaveManager::WaveManager()
    : currentWave(1), waveTimer(0), spawnTimer(0), enemiesSpawned(0), enemiesToSpawn(5),
      spawnInterval(2.0f), healthMultiplier(1.0f), damageMultiplier(1.0f) {}

void WaveManager::Update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
    spawnTimer += deltaTime;
    
    if (enemiesSpawned < enemiesToSpawn && spawnTimer >= spawnInterval) {
        SpawnEnemy(enemies);
        spawnTimer = 0;
    }
    
    // Check if wave is complete
     bool allEnemiesDead = true;
    for (auto& enemy : enemies) {
        if (enemy->IsAlive()) {
            allEnemiesDead = false;
            break;
        }
    }
    
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
            enemies.back()->ScaleStats(healthMultiplier, damageMultiplier);
            break;
        case 1:
            enemies.push_back(std::make_unique<SodaCan>(spawnPos));
            enemies.back()->ScaleStats(healthMultiplier, damageMultiplier);
            break;
        case 2:
            enemies.push_back(std::make_unique<PizzaBox>(spawnPos));
            enemies.back()->ScaleStats(healthMultiplier, damageMultiplier);
            break;
    }
    
    enemiesSpawned++;
}

void WaveManager::NextWave() {
    currentWave++;
    enemiesSpawned = 0;
    enemiesToSpawn += 2;
    healthMultiplier *= 1.2f;
    damageMultiplier *= 1.1f;
    spawnInterval = std::max(0.5f, spawnInterval * 0.95f);
}

int WaveManager::GetCurrentWave() const {
    return currentWave;
}