#pragma once
#include <string>

class Ability {
protected:
    std::string name;
    float cooldown;
    float currentCooldown;
    int damage;
    
public:
    Ability(std::string n, float cd, int dmg);
    virtual ~Ability() = default;
    
    void Update(float deltaTime);
    bool CanUse() const;
    void Use();
    
    // Getters
    std::string GetName() const { return name; }
    float GetCooldown() const { return cooldown; }
    float GetCurrentCooldown() const { return currentCooldown; }
    int GetDamage() const { return damage; }

protected:
    virtual void Execute() = 0;
};