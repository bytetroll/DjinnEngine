#pragma once

#include "IMemoryResource.h"
#include "StructuredBufferDesc.h"

namespace Djinn {
    namespace GHA {
        class IStructuredBuffer : public IMemoryResource {
        public:
            DJINN_COM_INTERFACE();

            IStructuredBuffer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMemoryResource(registry, outer, host) {

            }

            // Get description
            virtual const StructuredBufferDesc& GetDesc() = 0;

            /// Overrides
            virtual COM::InterfaceID GetIID() override {
                return kIID;
            }
        };
    }
}