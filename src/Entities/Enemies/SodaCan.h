#pragma once
#include "../Enemy.h"

class SodaCan : public Enemy {
public:
    SodaCan(Vector2 pos);
    void Draw() const override;
};