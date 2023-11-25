#pragma once

#include <vector>

#include "Math/MathUtils.h"

class StarrySky {
public:
    struct Star {
        Vector2 location;
        float baseSize;
        float seed;
    };

    void Initialize();
    void Update();
    void Regenerate();

    void SetVolume(float volume) { volume_ = volume; }

private:
    uint32_t numHorizontalGrids_ = 16;
    uint32_t numVerticalGrids_ = 16;

    std::vector<Star> stars_;
    float volume_ = 0.0f;
};