#include "Ability.h"

Ability::Ability(std::string n, float cd, int dmg) 
    : name(n), cooldown(cd), currentCooldown(0), damage(dmg) {}

void Ability::Update(float deltaTime) {
    if (currentCooldown > 0) {
        currentCooldown -= deltaTime;
    }
}

bool Ability::CanUse() const { 
    return currentCooldown <= 0; 
}

void Ability::Use() {
    if (CanUse()) {
        currentCooldown = cooldown;
        Execute();
    }
}