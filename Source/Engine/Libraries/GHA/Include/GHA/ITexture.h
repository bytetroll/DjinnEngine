#pragma once

#include "IMemoryResource.h"
#include "TextureDesc.h"

namespace Djinn {
    namespace GHA {
        class ITexture : public IMemoryResource {
        public:
            DJINN_COM_INTERFACE();

            ITexture(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMemoryResource(registry, outer, host) {

            }

            virtual ~ITexture() {

            }

            // Get description
            virtual const TextureDesc& GetDesc() = 0;

            /// Overrides
            virtual COM::InterfaceID GetIID() override {
                return kIID;
            }
        };
    }
}