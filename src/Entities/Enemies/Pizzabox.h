#pragma once
#include "../Enemy.h"

class PizzaBox : public Enemy {
public:
    PizzaBox(Vector2 pos);
    void Draw() const override;
};