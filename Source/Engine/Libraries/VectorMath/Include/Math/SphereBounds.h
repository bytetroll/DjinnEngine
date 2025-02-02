//
// Created by Dev on 8/26/2018.
//
#pragma once

#include "Vector3.h"

namespace Djinn::Math {
    struct SphereBounds {
        SphereBounds() = default;

        SphereBounds(const Math::Vector3& position, float radius) : position(position), radius(radius) {

        }

        /// Position of this sphere
        Math::Vector3 position;

        /// Radius of this sphere
        float radius = 0.0f;
    };
}