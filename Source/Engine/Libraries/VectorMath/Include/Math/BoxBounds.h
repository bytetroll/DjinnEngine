//
// Created by Dev on 8/26/2018.
//
#pragma once

#include "Vector3.h"

namespace Djinn::Math {
    struct BoxBounds {
        BoxBounds() = default;

        BoxBounds(const Math::Vector3& position, const Math::Vector3& extends) : position(position), extends(extends) {

        }

        /// Position of this box
        Math::Vector3 position;

        /// Extends of this box
        Math::Vector3 extends;
    };
}