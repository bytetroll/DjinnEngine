//
// Created by Dev on 8/26/2018.
//
#pragma once

#include "BoxBounds.h"
#include "SphereBounds.h"

namespace Djinn::Math {
    struct Bounds {
        Bounds() = default;

        Bounds(const BoxBounds& box) : box(box) {
            sphere = SphereBounds(box.position, Math::Max(box.extends.x, box.extends.y, box.extends.z));
        }

        Bounds(const SphereBounds& sphere) : sphere(sphere) {
            box = BoxBounds(box.position, {sphere.radius, sphere.radius, sphere.radius});
        }

        /// Box representation
        BoxBounds box;

        /// Sphere representation
        SphereBounds sphere;
    };
}