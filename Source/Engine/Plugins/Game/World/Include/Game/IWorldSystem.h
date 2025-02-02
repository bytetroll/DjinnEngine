#pragma once

#include <Game/IStepSystem.h>

namespace Djinn::Game {
    class DJINN_EXPORT_PLUGINS_GAME_WORLD IWorldSystem : public IStepSystem {
    public:
        DJINN_COM_INTERFACE();

        IWorldSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IStepSystem(registry, outer, host) {

        }

        /// Overrides
        virtual COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case IStepSystem::kIID:
                    *out = static_cast<IStepSystem*>(this);
                    return COM::kOK;
                case kIID:
                    *out = static_cast<IWorldSystem*>(this);
                    return COM::kOK;
            }
            return COM::kUnknownInterface;
        }
    };
}