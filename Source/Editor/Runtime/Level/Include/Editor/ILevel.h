//
// Created by Dev on 6/28/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include "ICommandProcessor.h"

namespace Djinn::Host {
    class IPipeline;
}

namespace Djinn::Game {
    class IWorld;
}

namespace Djinn::Editor {
    class ICommandHost;
    class IViewportCamera;

    class ILevel : public COM::TUnknown<ILevel>, public ICommandProcessor {
    public:
        DJINN_COM_INTERFACE();

        ILevel(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Get the game world
        /// \return
        virtual Game::IWorld* GetGameWorld() = 0;

        /// Get the internally hosted pipeline
        /// \return
        virtual Host::IPipeline *GetPipeline() = 0;

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case COM::IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case ILevel::kIID:
                    *out = static_cast<ILevel*>(this);
                    return COM::kOK;
                case ICommandProcessor::kIID:
                    *out = static_cast<ICommandProcessor*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }
    };
}