#include "Weapon.h"

Weapon::Weapon(std::string n, int dmg, float speed, std::string effect) 
    : name(n), damage(dmg), attackSpeed(speed), specialEffect(effect) {}

void Weapon::Attack(Vector2 position, Vector2 direction) {
    // Base attack implementation
    // Can be overridden by derived classes
}