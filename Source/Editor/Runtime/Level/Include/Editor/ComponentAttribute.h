//
// Created by Dev on 7/15/2018.
//
#pragma once

#include <Core/BitSet.h>
#include <Core/Types.h>

namespace Djinn::Editor {
    enum class ComponentAttribute : UInt16 {
        eNone = 1,

        /// Signifies that the component is active in the editor
        eRunInEditor = eNone << 1,
    };

    DJINN_CORE_BITSET(ComponentAttribute);
}