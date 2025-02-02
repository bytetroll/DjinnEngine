//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 8/30/2018.
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