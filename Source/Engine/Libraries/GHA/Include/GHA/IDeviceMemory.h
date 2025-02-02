#pragma once

#include <COM/IUnknown.h>

namespace Djinn {
    namespace GHA {
        class IDeviceMemory : public COM::TUnknown<IDeviceMemory> {
        public:
            DJINN_COM_INTERFACE();

            /// Construct
            /// \param registry
            /// \param outer
            IDeviceMemory(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

            }

            /// Get the total amount of memory allocated
            /// \return
            virtual USize GetAllocatedMemory() = 0;

            /// Get the total amount of memory used
            /// \return
            virtual USize GetUsedMemory() = 0;
        };
    }
}