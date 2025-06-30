#pragma once
#include <raylib.h>

struct GameTransform {
    Vector2 position {0, 0};
    Vector2 scale {1, 1};
    float rotation {0};
    
    GameTransform() = default;
    GameTransform(Vector2 pos, Vector2 sc = {1, 1}, float rot = 0) 
        : position(pos), scale(sc), rotation(rot) {}
};