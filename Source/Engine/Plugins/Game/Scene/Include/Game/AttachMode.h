//
// Created by Dev on 2/25/2018.
//
#pragma once

#include <Core/Types.h>

namespace Djinn::Game {
    enum class AttachMode : UInt8 {
        /// No effect, parent is only being held as a reference
                eReference,

        /// The transformation of the parent is carried on
                eTransform,

        /// Same effect as eTransform, but upon destruction of the parent transform the child entity will be destroyed
                eChild,
    };
}