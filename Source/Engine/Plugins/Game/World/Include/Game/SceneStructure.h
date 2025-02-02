//
// Created by Dev on 8/27/2018.
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