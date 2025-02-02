//
// Created by Dev on 2/25/2018.
//
#pragma once

#include <Core/Types.h>

namespace Djinn::UI {
    enum class UIContentMode : UInt8 {
        // Element does not contribute to content
                eNone,

        // Element contributes to content
                eInclude,
    };
}