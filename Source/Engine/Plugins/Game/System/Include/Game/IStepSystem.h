#pragma once

#include <Base/ISystem.h>
#include "StepMode.h"

namespace Djinn {
    namespace Game {
        class IStepSystem : public Base::ISystem {
        public:
            DJINN_COM_INTERFACE();

            IStepSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISystem(registry, outer, host) {

            }

            /// Override
            COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
                switch (iid) {
                    case COM::IUnknown::kIID:
                        *out = static_cast<COM::IUnknown*>(this);
                        return COM::kOK;
                    case Base::ISystem::kIID:
                        *out = static_cast<Base::ISystem*>(this);
                        return COM::kOK;
                    case IStepSystem::kIID:
                        *out = static_cast<IStepSystem*>(this);
                        return COM::kOK;
                }
                return COM::kUnknownInterface;
            }

            /// Run the system
            /// \return
            virtual COM::Result Run(StepMode mode, double delta) = 0;

            /// Get step mode
            /// \return
            virtual StepModeSet GetStepMode() = 0;

            /// Asynchronous methods
            Asyncf(Run)
        };
    }
}