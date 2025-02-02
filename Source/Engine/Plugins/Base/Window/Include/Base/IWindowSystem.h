//
// Created by Dev on 2/25/2018.
//
#pragma once

#include <Base/IApplicationSystem.h>

namespace Djinn::Platform {
    class Window;
}

namespace Djinn::Base {
    class IWindowSystem : public IApplicationSystem {
    public:
        DJINN_COM_INTERFACE();

        /// Construct
        /// \param registry
        /// \param outer
        IWindowSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IApplicationSystem(registry, outer, host) {

        }

        /// Get platform window
        /// \return
        virtual Platform::Window *GetWindow() = 0;

        /// Overrides
        virtual COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case COM::IUnknown::kIID:
                    *out = static_cast<COM::IUnknown*>(this);
                    return COM::kOK;
                case ISystem::kIID:
                    *out = static_cast<ISystem*>(this);
                    return COM::kOK;
                case kIID:
                    *out = static_cast<IWindowSystem*>(this);
                    return COM::kOK;
            }
            return COM::kUnknownInterface;
        }
    };
}