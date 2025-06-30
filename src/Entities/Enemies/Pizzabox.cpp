#include "PizzaBox.h"
#include "raylib.h"

PizzaBox::PizzaBox(Vector2 pos) : Enemy(EnemyType::PIZZA_BOX, 40, 8, 1.5f, pos) {}

void PizzaBox::Update(float deltaTime) {}
void PizzaBox::Draw() const {
    Vector2 screenPos = {400 + transform.position.x, 300 + transform.position.y};
    Color c = hurtTimer > 0 ? RED : ORANGE;
    DrawRectangle(screenPos.x - 16, screenPos.y - 4, 32, 8, c);
    DrawText("P", screenPos.x - 6, screenPos.y - 8, 16, BLACK);
}