#pragma once
#include "Entity.h"
#include "GameState.h"
#include "EnemyType.h"
#include "Utils.h"

class Player;

class Enemy : public Entity {
protected:
    EnemyType type;
    int health;
    int maxHealth;
    int damage;
    float speed;
    Vector2 gridPosition;
    float attackCooldown;
    float attackTimer;
    float hurtTimer;

public:
    Enemy(EnemyType t, int hp, int dmg, float spd, Vector2 pos);

    virtual void Update(float deltaTime, Player* player);
    virtual void Draw() const override = 0;

    virtual void Attack(Player* player);

    void TakeDamage(int dmg);
    bool CheckCollision(const Player* player) const;

    void SetGridPosition(Vector2 pos) { gridPosition = pos; transform.position = Utils::WorldToIso(gridPosition); }
    void ScaleStats(float hpMult, float dmgMult) {
        health = static_cast<int>(health * hpMult);
        maxHealth = static_cast<int>(maxHealth * hpMult);
        damage = static_cast<int>(damage * dmgMult);
    }
    
    // Getters
    int GetDamage() const { return damage; }
    EnemyType GetType() const { return type; }
    int GetHealth() const { return health; }
    int GetMaxHealth() const { return maxHealth; }
    bool IsAlive() const { return isActive; }
    Vector2 GetGridPosition() const { return gridPosition; }
};