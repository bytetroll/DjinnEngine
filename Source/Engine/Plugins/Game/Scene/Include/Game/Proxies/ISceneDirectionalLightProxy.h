//
// Created by Dev on 8/27/2018.
//
#pragma once

#include <Game/ISceneProxy.h>
#include <Game/ISceneProxySystem.h>
#include <Graphics/SceneDirectionalLightPacket.h>

namespace Djinn::Game {
    class ISceneDirectionalLightProxy : public ISceneProxy {
    public:
        DJINN_COM_INTERFACEID();

        /// Build this proxy
        /// \param buffer
        virtual void Build(const Core::ArrayView<SceneObject> &requests, Graphics::SceneDirectionalLightPacketBuffer *buffer) = 0;

        /// Overrides
        COM::InterfaceID GetInterfaceID() override {
            return kIID;
        }
    };

    class DJINN_EXPORT_PLUGINS_GAME_SCENE SceneDirectionalLightProxySystem : public ISceneProxySystem {
    public:
        DJINN_COM_CLASS();

        SceneDirectionalLightProxySystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void Build(COM::IQueryable *queryable, const Core::ArrayView<SceneProxyRequest> &requests) override;
        COM::InterfaceID GetProxyInterfaceID() override;
    };
}
