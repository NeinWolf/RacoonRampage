#include "Player.h"
#include "raylib.h"
#include "Utils.h" // For WorldToIso
#include <cmath>

Player::Player() : 
    health(100), maxHealth(100), 
    stamina(50), maxStamina(50), 
    scraps(0), gridPosition({5, 5}), lastMoveDir({0, 1}) {
    
    // Initialize animations
    std::vector<Rectangle> idleFrames = {{0, 0, 32, 32}, {32, 0, 32, 32}};
    std::vector<Rectangle> attackFrames = {{0, 32, 32, 32}, {32, 32, 32, 32}, {64, 32, 32, 32}};
    std::vector<Rectangle> hurtFrames = {{0, 64, 32, 32}};
    
    animations["idle"] = new Animation(idleFrames, 0.5f);
    animations["attack"] = new Animation(attackFrames, 0.2f, false);
    animations["hurt"] = new Animation(hurtFrames, 0.3f, false);
    
    currentAnimation = animations["idle"];
    
    weapon = std::make_unique<Weapon>("Bottle", 10, 1.0f);
}

void Player::Update(float deltaTime) {
    currentAnimation->Update(deltaTime);
    
    // Update abilities
    for (auto& ability : abilities) {
        ability->Update(deltaTime);
    }
    
    // Handle input
    Vector2 movement = {0, 0};
    if (IsKeyDown(KEY_W)) movement.y -= 1;
    if (IsKeyDown(KEY_S)) movement.y += 1;
    if (IsKeyDown(KEY_A)) movement.x -= 1;
    if (IsKeyDown(KEY_D)) movement.x += 1;
    
    if (movement.x != 0 || movement.y != 0) {
        float len = sqrtf(movement.x * movement.x + movement.y * movement.y);
        if (len > 0) {
            lastMoveDir = { movement.x / len, movement.y / len };
        }
        gridPosition.x += movement.x * deltaTime * 3;
        gridPosition.y += movement.y * deltaTime * 3;
        
        // Keep player in bounds
        gridPosition.x = std::fmax(0, fmin(20, gridPosition.x));
        gridPosition.y = std::fmax(0, fmin(20, gridPosition.y));
    }
    
    transform.position = Utils::WorldToIso(gridPosition);
}

void Player::Draw() const {
    Vector2 screenPos = {400 + transform.position.x, 300 + transform.position.y};
    DrawRectangle(screenPos.x - 16, screenPos.y - 16, 32, 32, GREEN);
    DrawText("R", screenPos.x - 6, screenPos.y - 8, 20, WHITE);
}

void Player::TakeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        health = 0;
        isActive = false;
    }
    currentAnimation = animations["hurt"];
    currentAnimation->Reset();
}

void Player::Attack() {
    currentAnimation = animations["attack"];
    currentAnimation->Reset();
    weapon->Attack(transform.position, lastMoveDir);
}

void Player::UseAbility(int index) {
    if (index >= 0 && index < static_cast<int>(abilities.size())) {
        abilities[index]->Use();
    }
}