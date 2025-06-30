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

    // Convert isometric screen coordinates back to world grid coordinates
    inline Vector2 IsoToWorld(Vector2 isoPos) {
        return {
            (isoPos.x / 32.0f + isoPos.y / 16.0f) * 0.5f,
            (isoPos.y / 16.0f - isoPos.x / 32.0f) * 0.5f
        };
    }

    // Clamp value between min and max
    template<typename T>
    inline T Clamp(T value, T min, T max) {
        return std::max(min, std::min(value, max));
    }
}