#include "Animation.h"

Animation::Animation(std::vector<Rectangle> frameRects, float time, bool shouldLoop) 
    : frames(frameRects), currentFrame(0), frameTime(time), timer(0), loop(shouldLoop) {}

void Animation::Update(float deltaTime) {
    timer += deltaTime;
    if (timer >= frameTime) {
        timer = 0;
        currentFrame++;
        if (currentFrame >= static_cast<int>(frames.size())) {
            if (loop) currentFrame = 0;
            else currentFrame = static_cast<int>(frames.size()) - 1;
        }
    }
}

Rectangle Animation::GetCurrentFrame() const {
    return frames[currentFrame];
}

void Animation::Reset() {
    currentFrame = 0;
    timer = 0;
}

bool Animation::IsComplete() const {
    return !loop && currentFrame >= static_cast<int>(frames.size()) - 1;
}