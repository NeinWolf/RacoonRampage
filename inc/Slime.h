#pragma once
#include "Enemy.h"

class Slime : public Enemy {
public:
    Slime(Vector2 pos);
    void Update(float deltaTime) override;
    void Draw() const override;
};