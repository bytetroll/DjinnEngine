//
// Created by Dev on 8/26/2018.
//
#pragma once

#include <Math/Bounds.h>

namespace Djinn::Game {
    struct SceneObject {
        /// Set the bounds of this object
        /// \param bounds
        /// \return
        SceneObject& SetBounds(const Math::Bounds& bounds) {
            this->bounds = bounds;
            return *this;
        }

        /// Set the index of this object
        /// \param index
        /// \return
        SceneObject& SetIndex(UInt32 index) {
            this->index = index;
            return *this;
        }

        /// Bounds of this object
        Math::Bounds bounds;

        /// Index of this object
        UInt32 index = 0;
    };
}
