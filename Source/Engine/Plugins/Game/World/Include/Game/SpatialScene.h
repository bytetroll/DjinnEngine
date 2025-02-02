//
// Created by Dev on 8/27/2018.
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