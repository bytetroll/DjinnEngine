//
// Created by Dev on 8/23/2018.
//
#pragma once

#include <Game/IWorldSystem.h>

namespace Djinn::UI {
    class IUICanvas;

    class IUIInputSystem : public Game::IWorldSystem {
    public:
        DJINN_COM_INTERFACE();

        IUIInputSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldSystem(registry, outer, host) {

        }

        /// Register a canvas
        /// \param canvas
        virtual void Register(IUICanvas *canvas) = 0;

        /// Deregister a canvas
        /// \param canvas
        virtual void Deregister(IUICanvas *canvas) = 0;
    };
}
