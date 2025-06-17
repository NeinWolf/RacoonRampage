#include "Projectile.h"
#include "raylib.h"

Projectile::Projectile(Vector2 position, Vector2 dir, float spd, int dmg) 
    : direction(dir), speed(spd), damage(dmg), active(true) {
    transform.position = position;
    transform.scale = {16, 16};
}

void Projectile::Update(float deltaTime) {
    transform.position.x += direction.x * speed * deltaTime;
    transform.position.y += direction.y * speed * deltaTime;
    
    // Deactivate if out of bounds
    if (transform.position.x < 0 || transform.position.x > 800 ||
        transform.position.y < 0 || transform.position.y > 600) {
        active = false;
    }
}

void Projectile::Draw() const {
    DrawRectangleV(transform.position, transform.scale, YELLOW);
}

bool Projectile::IsActive() const { 
    return active; 
}

int Projectile::GetDamage() const { 
    return damage; 
}

Rectangle Projectile::GetHitbox() const {
    return {transform.position.x, transform.position.y, 
            transform.scale.x, transform.scale.y};
}