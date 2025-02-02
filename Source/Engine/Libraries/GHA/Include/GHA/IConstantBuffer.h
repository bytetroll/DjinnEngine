#pragma once

#include "ConstantBufferDesc.h"
#include "IMemoryResource.h"

namespace Djinn {
    namespace GHA {
        class IConstantBuffer : public IMemoryResource {
        public:
            DJINN_COM_INTERFACE();

            IConstantBuffer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMemoryResource(registry, outer, host) {

            }

            // Get description
            virtual const ConstantBufferDesc& GetDesc() = 0;

            /// Overrides
            virtual COM::InterfaceID GetIID() override {
                return kIID;
            }
        };
    }
}