#pragma once

#include <COM/IUnknown.h>
#include "InstanceDesc.h"
#include "DeviceDesc.h"

namespace Djinn {
    namespace GHA {
        class IAdapter;
        class IDevice;

        class IInstance : public COM::TUnknown<IInstance> {
        public:
            DJINN_COM_INTERFACE();

            IInstance(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

            }

            /// Initialize this instance
            /// \return
            virtual COM::Result Initialize(const InstanceDesc& desc) = 0;

            /// Get adapters
            /// \param count
            /// \param adapters
            /// \return
            virtual COM::Result GetAdapters(UInt32& count, IAdapter** adapters) = 0;

            /// Create a device
            /// \param adapter
            /// \param out
            /// \return
            virtual COM::Result CreateDevice(COM::IUnknown* outer, IAdapter* adapter, const DeviceDesc& desc, IDevice** out) = 0;
        };
    }
}