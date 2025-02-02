//
// Created by Dev on 7/12/2018.
//
#pragma once

#include <Core/BitSet.h>

namespace Djinn::Editor {
    enum class CommandFlag {
        eNone = 0,
        eSkip = static_cast<int>(eNone) << 1,
        eSkipIfFail = (static_cast<int>(eSkip) << 1) | eSkip,
    };

    DJINN_CORE_BITSET(CommandFlag);
}