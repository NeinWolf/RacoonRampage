#pragma once
#include "Entity.h"
#include "../Game/GameState.h"
#include "EnemyType.h"

class Player;

class Enemy : public Entity {
protected:
    EnemyType type;
    int health;
    int maxHealth;
    int damage;
    float speed;
    Vector2 gridPosition;

public:
    Enemy(EnemyType t, int hp, int dmg, float spd, Vector2 pos);
    
    virtual void Update(float deltaTime, Player* player);
    virtual void Draw() const override = 0;
    
    void TakeDamage(int dmg);
    bool CheckCollision(const Player* player) const;
    
    // Getters
    int GetDamage() const { return damage; }
    EnemyType GetType() const { return type; }
    int GetHealth() const { return health; }
    int GetMaxHealth() const { return maxHealth; }
    bool IsAlive() const { return isActive; }
    Vector2 GetGridPosition() const { return gridPosition; }
};