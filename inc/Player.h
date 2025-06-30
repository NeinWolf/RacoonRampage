#pragma once
#include "Entity.h"
#include "Ability.h"
#include "Weapon.h"
#include <vector>
#include <memory>

class Player : public Entity {
private:
    int health;
    int maxHealth;
    int stamina;
    int maxStamina;
    int scraps;
    Vector2 gridPosition;
    Vector2 lastMoveDir;
    std::unique_ptr<Weapon> weapon;
    std::vector<std::unique_ptr<Ability>> abilities;

public:
    Player();
    
    void Update(float deltaTime) override;
    void Draw() const override;
    
    void TakeDamage(int damage);
    void Attack();
    void UseAbility(int index);

    Vector2 GetLastMoveDir() const { return lastMoveDir; }
    
    // Getters
    int GetHealth() const { return health; }
    int GetMaxHealth() const { return maxHealth; }
    int GetStamina() const { return stamina; }
    int GetMaxStamina() const { return maxStamina; }
    int GetScraps() const { return scraps; }
    Vector2 GetGridPosition() const { return gridPosition; }
    const std::vector<std::unique_ptr<Ability>>& GetAbilities() const { return abilities; }
    const Weapon* GetWeapon() const { return weapon.get(); }
    
    // Setters
    void AddScraps(int amount) { scraps += amount; }
};