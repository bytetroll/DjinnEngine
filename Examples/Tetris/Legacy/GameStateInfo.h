#pragma once

#include <Core/String.h>

#include "GameState.h"

namespace Tetris {
    using namespace Djinn;

    typedef struct {
        Core::String name;
        GameState* state;
    } GameStateInfo;
}