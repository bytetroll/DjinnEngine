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
 on 8/27/2018.
//
#pragma once

#include "ISceneStructure.h"
#include "SceneBuilder.h"
#include "SceneStructureViewport.h"
#include <Core/List.h>
#include <Core/Map.h>
#include <Core/SlotArray.h>

namespace Djinn::Game {
    class DJINN_EXPORT_PLUGINS_GAME_WORLD SceneStructure : public ISceneStructure {
    public:
        DJINN_COM_CLASS();

        SceneStructure(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this structure
        /// \return
        COM::Result Initialize();

        /// Overrides
        COM::Result Build() override;
        COM::Result BuildViewport(COM::IQueryable *queryable, SceneStructureViewport* viewport, const Math::Matrix4 &transform) override;
        void SetOcclusion(ISceneOcclusion *occlusion) override;
        ISceneOcclusion *GetOcclusion() override;
        void Register(ISceneProxy *proxy) override;
        void Deregister(ISceneProxy *proxy) override;

    private:
        ISceneOcclusion* occlusion = nullptr;
        Core::Buffer<SceneObject> proxyObjectBuffer;

    private:
        struct InterfaceState {
            COM::InterfaceID iid;

            Core::SlotArray<ISceneProxy*> proxies;

            struct StaticHeader {
                USize offset;
                UInt32 count;
            };

            Core::SlotArray<StaticHeader> staticHeaders;
            Core::Buffer<SceneProxyRequest> staticRequests;

            Core::Buffer<SceneProxyRequest> dynamicRequests;
        };

        InterfaceState* GetInterfaceState(const COM::InterfaceID& iid);

        Core::Map<COM::InterfaceID, InterfaceState*> interfaceMap;
        Core::List<InterfaceState> interfaceStates;

        SceneBuilder builder;
    };
}