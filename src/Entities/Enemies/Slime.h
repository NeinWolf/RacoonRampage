#pragma once
#include "../Enemy.h"

class Slime : public Enemy {
public:
    Slime(Vector2 pos);
    void Draw() const override;
};