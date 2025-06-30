#include "Slime.h"
#include "raylib.h"

Slime::Slime(Vector2 pos) : Enemy(EnemyType::SLIME, 30, 10, 2.0f, pos) {}

void Slime::Update(float deltaTime) {}
void Slime::Draw() const {
    Vector2 screenPos = {400 + transform.position.x, 300 + transform.position.y};
    Color c = hurtTimer > 0 ? RED : GREEN;
    DrawCircle(screenPos.x, screenPos.y, 16, c);
    DrawText("S", screenPos.x - 6, screenPos.y - 8, 16, BLACK);
}