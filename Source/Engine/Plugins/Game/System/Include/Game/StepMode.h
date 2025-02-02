//
// Created by Dev on 1/10/2018.
//
#pragma once

#include <Core/BitSet.h>

namespace Djinn::Game {
    enum class StepMode {
        eOnce = 0x1,
        eAsync = static_cast<int>(eOnce) << 1,
        eIntegral = static_cast<int>(eAsync) << 1,
        eAll = static_cast<int>(eOnce) | static_cast<int>(eIntegral)
    };

    DJINN_CORE_BITSET(StepMode);
}