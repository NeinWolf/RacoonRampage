#pragma once
#include <string>
#include "raylib.h"

class Weapon {
protected:
    std::string name;
    int damage;
    float attackSpeed;
    std::string specialEffect;
    
public:
    Weapon(std::string n, int dmg, float speed, std::string effect = "");
    virtual ~Weapon() = default;
    
    virtual void Attack(Vector2 position, Vector2 direction);
    
    // Getters
    std::string GetName() const { return name; }
    int GetDamage() const { return damage; }
    float GetAttackSpeed() const { return attackSpeed; }
    std::string GetSpecialEffect() const { return specialEffect; }
};