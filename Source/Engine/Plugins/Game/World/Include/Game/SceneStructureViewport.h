//
// Created by Dev on 8/30/2018.
//
#pragma once

#include <Core/Buffer.h>
#include <COM/ID.h>
#include <Core/ArrayView.h>
#include "SceneObject.h"

namespace Djinn::Game {
    class ISceneProxy;

    /// A basic proxy request
    struct SceneProxyRequest {
        ISceneProxy* proxy;
        Core::ArrayView<SceneObject> objects;
    };

    struct SceneStructureViewport {
        /// Clear all interface data
        void ClearInterfaces() {
            for (auto&& interface : interfaces) {
                interface.requests.Clear();
            }
        }

        /// Viewport of an interface
        struct InterfaceViewport {
            COM::InterfaceID iid;
            Core::Buffer<SceneProxyRequest> requests;
        };

        Core::Array<InterfaceViewport> interfaces;
    };
}