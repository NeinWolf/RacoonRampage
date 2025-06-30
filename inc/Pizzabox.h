#pragma once
#include "Enemy.h"

class PizzaBox : public Enemy {
public:
    PizzaBox(Vector2 pos);
    void Update(float deltaTime) override;
    void Draw() const override;
};