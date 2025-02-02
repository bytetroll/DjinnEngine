#pragma once

#include "IMemoryResource.h"
#include "IResource.h"
#include "IndexBufferDesc.h"

namespace Djinn {
    namespace GHA {
        class IIndexBuffer : public IMemoryResource {
        public:
            DJINN_COM_INTERFACE();

            IIndexBuffer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMemoryResource(registry, outer, host) {

            }

            // Get description
            virtual const IndexBufferDesc& GetDesc() = 0;

            /// Overrides
            virtual COM::InterfaceID GetIID() override {
                return kIID;
            }
        };
    }
}