//
// Created by Dev on 8/25/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Core/ArrayView.h>
#include <Async/Async.h>
#include "SceneStructureViewport.h"

namespace Djinn::Game {
    class ISceneProxy;

    class ISceneProxySystem : public COM::TUnknown<ISceneProxySystem> {
    public:
        DJINN_COM_INTERFACE();

        ISceneProxySystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Build a set of proxies
        /// \param proxies
        virtual void Build(COM::IQueryable* queryable, const Core::ArrayView<SceneProxyRequest>& requests) = 0;

        /// Get the proxy interface id this system handles
        /// \return
        virtual COM::InterfaceID GetProxyInterfaceID() = 0;

        /// Async helpers
        Asyncf(Build);
    };
}
