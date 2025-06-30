#pragma once
#include "raylib.h"
#include <algorithm>

namespace Utils {
    // Convert world coordinates to isometric screen coordinates
    inline Vector2 WorldToIso(Vector2 worldPos) {
        return {
            (worldPos.x - worldPos.y) * 32.0f,
            (worldPos.x + worldPos.y) * 16.0f
        };
    }

    // Clamp value between min and max
    template<typename T>
    inline T Clamp(T value, T min, T max) {
        return std::max(min, std::min(value, max));
    }
}