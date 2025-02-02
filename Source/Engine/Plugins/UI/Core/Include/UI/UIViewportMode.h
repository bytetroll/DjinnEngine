//
// Created by Dev on 2/25/2018.
//
#pragma once

#include <Core/Types.h>

namespace Djinn::UI {
    enum class UIViewportMode : UInt8 {
        // Does not care about a viewport
        eNone,

        // Keeps the parents viewport
        eKeep,

        // Inherits the viewport
        eInherit
    };
}