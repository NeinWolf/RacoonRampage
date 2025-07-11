#pragma once
#include <vector>
#include <memory>
#include "Enemy.h"

class WaveManager {
private:
    int currentWave;
    float waveTimer;
    float spawnTimer;
    int enemiesSpawned;
    int enemiesToSpawn;
    float spawnInterval;
    float healthMultiplier;
    float damageMultiplier;
    
public:
    WaveManager();
    void Update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies);
    void SpawnEnemy(std::vector<std::unique_ptr<Enemy>>& enemies);
    void NextWave();
    int GetCurrentWave() const;
};