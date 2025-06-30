#include "Enemy.h"
#include "Player.h"
#include "raylib.h"
#include "../Game/Utils.h"
#include <cmath>

Enemy::Enemy(EnemyType t, int hp, int dmg, float spd, Vector2 pos) :
    type(t), health(hp), maxHealth(hp), 
    damage(dmg), speed(spd), gridPosition(pos) {}

void Enemy::Update(float deltaTime, Player* player) {
    if (!isActive) return;
    
    // Simple AI - move towards player
    Vector2 direction = {
        player->GetGridPosition().x - gridPosition.x,
        player->GetGridPosition().y - gridPosition.y
    };
    
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
        
        gridPosition.x += direction.x * speed * deltaTime;
        gridPosition.y += direction.y * speed * deltaTime;
    }
    
    transform.position = Utils::WorldToIso(gridPosition);
}

void Enemy::TakeDamage(int dmg) {
    health -= dmg;
    if (health <= 0) {
        health = 0;
        isActive = false;
    }
}

bool Enemy::CheckCollision(const Player* player) const {
    float distX = gridPosition.x - player->GetGridPosition().x;
    float distY = gridPosition.y - player->GetGridPosition().y;
    return (distX * distX + distY * distY) < 1.0f;
}