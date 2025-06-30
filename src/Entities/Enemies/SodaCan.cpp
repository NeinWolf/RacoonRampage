#include "SodaCan.h"
#include "raylib.h"

SodaCan::SodaCan(Vector2 pos) : Enemy(EnemyType::SODA_CAN, 20, 15, 3.0f, pos) {}

void SodaCan::Update(float deltaTime) {}
void SodaCan::Draw() const {
    Vector2 screenPos = {400 + transform.position.x, 300 + transform.position.y};
    DrawRectangle(screenPos.x - 8, screenPos.y - 12, 16, 24, RED);
    DrawText("C", screenPos.x - 6, screenPos.y - 8, 16, WHITE);
}