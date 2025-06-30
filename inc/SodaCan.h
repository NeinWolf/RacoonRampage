#pragma once
#include "Enemy.h"

class SodaCan : public Enemy {
public:
    SodaCan(Vector2 pos);
    void Update(float deltaTime) override;
    void Draw() const override;
};