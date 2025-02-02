//
// Created by Dev on 8/26/2018.
//
#pragma once

#include <Core/Buffer.h>
#include "SceneObject.h"

namespace Djinn::Game {
    struct SceneBuilder {
        /// Add an object
        /// \param object
        void Add(const SceneObject& object) {
            objects.Add(object);
        }

        /// Clear this builder
        void Clear() {
            objects.Clear();
        }

        Core::Buffer<SceneObject> objects;
    };
}
