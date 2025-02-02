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

#include "ISpatialScene.h"
#include "SceneStructureViewport.h"
#include <Core/Map.h>
#include <Async/Mutex.h>

namespace Djinn::Game {
    class DJINN_EXPORT_PLUGINS_GAME_WORLD SpatialScene : public ISpatialScene {
    public:
        DJINN_COM_CLASS();

        SpatialScene(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this scene
        /// \return
        COM::Result Initialize(IWorld* world, ISceneState* state) override;

        /// Overrides
        COM::Result Build() override;
        COM::Result BuildViewport(COM::IQueryable *queryable, const Math::Matrix4 &transform) override;
        COM::Result Register(ISceneProxySystem *proxy) override;
        COM::Result Deregister(ISceneProxySystem *proxy) override;
        void Register(ISceneProxy *proxy) override;
        void Deregister(ISceneProxy *proxy) override;
        void SetStructure(ISceneStructure *structure) override;
        ISceneStructure *GetStructure() override;
        IWorld *GetWorld() override;
        ISceneState *GetState() override;

    private:
        SceneStructureViewport* PopViewport();

    private:
        Core::Array<ISceneProxySystem*> systems;
        Core::Map<COM::InterfaceID, ISceneProxySystem*> systemMap;
        Async::Mutex viewportPoolMtx;
        Core::ObjectPool<SceneStructureViewport> viewportPool;
        ISceneStructure* structure = nullptr;
        IWorld* world = nullptr;
        ISceneState* state = nullptr;
    };
}