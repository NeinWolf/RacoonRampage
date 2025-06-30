#pragma once
#include "raylib.h"
#include <vector>

class Animation {
private:
    std::vector<Rectangle> frames;
    int currentFrame;
    float frameTime;
    float timer;
    bool loop;

public:
    Animation(std::vector<Rectangle> frameRects, float time, bool shouldLoop = true);
    void Update(float deltaTime);
    Rectangle GetCurrentFrame() const;
    void Reset();
    bool IsComplete() const;
};