#pragma once

#include <Base/ISystem.h>

namespace Djinn::Base {
    class IApplicationSystem : public ISystem {
    public:
        DJINN_COM_INTERFACE();

        IApplicationSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISystem(registry, outer, host) {

        }

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case COM::IUnknown::kIID:
                    *out = static_cast<COM::IUnknown*>(this);
                    return COM::kOK;
                case ISystem::kIID:
                    *out = static_cast<ISystem*>(this);
                    return COM::kOK;
                case IApplicationSystem::kIID:
                    *out = static_cast<IApplicationSystem*>(this);
                    return COM::kOK;
            }
            return COM::kUnknownInterface;
        }

        /// Run the system
        /// \return
        virtual COM::Result Run() = 0;

        /// Asynchronous methods
        Asyncf(Run);
    };
}