#pragma once

#include <cstdint>

namespace CollisionConfig {

    const uint32_t Player = 0x1 << 0;
    const uint32_t Stage = 0x1 << 1;
    const uint32_t Item = 0x1 << 2;
    const uint32_t Goal = 0x1 << 3;
    const uint32_t Entrance = 0x1 << 4;
}