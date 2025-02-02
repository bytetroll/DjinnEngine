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
 on 1/27/2019.
//
#pragma once

#include "IScene.h"
#include <Math/Matrix4.h>

namespace Djinn::Game {
    class ISceneStructure;
    class ISceneProxy;
    class ISceneProxySystem;

    class ISpatialScene : public IScene {
    public:
        DJINN_COM_INTERFACE();

        ISpatialScene(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : IScene(registry, outer, host) {

        }

        /// Build a viewport
        /// Scene must be built before building viewports
        /// TS: This method is thread safe as long as no other method than BuildViewport is being invoked
        /// \param queryable
        /// \param transform
        /// \return
        virtual COM::Result BuildViewport(COM::IQueryable *queryable, const Math::Matrix4 &transform) = 0;

        /// Register a scene proxy
        /// \param proxy
        virtual void Register(ISceneProxy *proxy) = 0;

        /// Deregister a scene proxy
        /// \param proxy
        virtual void Deregister(ISceneProxy *proxy) = 0;

        /// Register a scene proxy system
        /// \param proxy
        virtual COM::Result Register(ISceneProxySystem *proxy) = 0;

        /// Deregister a scene proxy system
        /// \param proxy
        virtual COM::Result Deregister(ISceneProxySystem *proxy) = 0;

        /// Set the scene structure
        /// \return
        virtual void SetStructure(ISceneStructure *structure) = 0;

        /// Get the scene structure
        /// \return
        virtual ISceneStructure *GetStructure() = 0;

        /// Overrides
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown *>(this);
                    return COM::kOK;
                case IScene::kIID:
                    *out = static_cast<IScene *>(this);
                    return COM::kOK;
                case ISpatialScene::kIID:
                    *out = static_cast<ISpatialScene *>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }
    };
}