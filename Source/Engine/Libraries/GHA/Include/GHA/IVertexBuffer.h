#pragma once

#include "IMemoryResource.h"
#include "IResource.h"
#include "VertexBufferDesc.h"

namespace Djinn {
    namespace GHA {
        class IVertexBuffer : public IMemoryResource {
        public:
            DJINN_COM_INTERFACE();

            /// Construct
            /// \param registry
            /// \param outer
            IVertexBuffer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMemoryResource(registry, outer, host) {

            }

            // Get description
            virtual const VertexBufferDesc& GetDesc() = 0;

            /// Overrides
            virtual COM::InterfaceID GetIID() override {
                return kIID;
            }
        };
    }
}