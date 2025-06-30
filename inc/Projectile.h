#pragma once
#include "raylib.h"
#include "GameTransform.h"

class Projectile {
private:
    GameTransform transform;
    Vector2 direction;
    float speed;
    int damage;
    bool active;

public:
    Projectile(Vector2 position, Vector2 dir, float spd, int dmg);
    
    void Update(float deltaTime);
    void Draw() const;
    bool IsActive() const;
    int GetDamage() const;
    Rectangle GetHitbox() const;
};