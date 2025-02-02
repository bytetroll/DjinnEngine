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
 on 8/25/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Math/Matrix4.h>
#include <Core/ArrayView.h>

namespace Djinn::Game {
    class ISceneOcclusion;
    class ISceneProxy;
    struct SceneObject;
    struct SceneStructureViewport;

    class ISceneStructure : public COM::TUnknown<ISceneStructure> {
    public:
        DJINN_COM_INTERFACE();

        ISceneStructure(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Build the structure
        /// \return
        virtual COM::Result Build() = 0;

        /// Build a viewport
        /// Structure must be built before building viewports
        /// TS: This method is thread safe as long as no other method than BuildViewport is being invoked
        /// \param queryable
        /// \param transform
        /// \return
        virtual COM::Result BuildViewport(COM::IQueryable *queryable, SceneStructureViewport* viewport, const Math::Matrix4& transform) = 0;

        /// Set the scene occlusion
        /// \return
        virtual void SetOcclusion(ISceneOcclusion* occlusion) = 0;

        /// Get the scene occlusion
        /// \return
        virtual ISceneOcclusion* GetOcclusion() = 0;

        /// Register a scene proxy
        /// \param proxy
        virtual void Register(ISceneProxy *proxy) = 0;

        /// Deregister a scene proxy
        /// \param proxy
        virtual void Deregister(ISceneProxy *proxy) = 0;
    };
}