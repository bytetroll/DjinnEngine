#pragma once

#include "IMemoryResource.h"
#include "TextureBufferDesc.h"

namespace Djinn {
    namespace GHA {
        class ITextureBuffer : public IMemoryResource {
        public:
            DJINN_COM_INTERFACE();

            ITextureBuffer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMemoryResource(registry, outer, host) {

            }

            // Get description
            virtual const TextureBufferDesc& GetDesc() = 0;

            /// Overrides
            virtual COM::InterfaceID GetIID() override {
                return kIID;
            }
        };
    }
}