#include "Enemy.h"
#include "Player.h"
#include "raylib.h"
#include "Utils.h"
#include <cmath>

Enemy::Enemy(EnemyType t, int hp, int dmg, float spd, Vector2 pos) :
    type(t), health(hp), maxHealth(hp),
    damage(dmg), speed(spd), gridPosition(pos),
    attackCooldown(1.0f), attackTimer(0.0f), hurtTimer(0.0f) {}

void Enemy::Update(float deltaTime, Player* player) {
    if (!isActive) return;

    attackTimer += deltaTime;
    if (hurtTimer > 0.0f) {
        hurtTimer -= deltaTime;
    }
    
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

    if (CheckCollision(player) && attackTimer >= attackCooldown) {
        Attack(player);
        attackTimer = 0.0f;
    }
}

void Enemy::TakeDamage(int dmg) {
    health -= dmg;
    hurtTimer = 0.2f;
    if (health <= 0) {
        health = 0;
        isActive = false;
    }
}

void Enemy::Attack(Player* player) {
    if (player)
        player->TakeDamage(damage);
}

bool Enemy::CheckCollision(const Player* player) const {
    float distX = gridPosition.x - player->GetGridPosition().x;
    float distY = gridPosition.y - player->GetGridPosition().y;
    return (distX * distX + distY * distY) < 1.0f;
}