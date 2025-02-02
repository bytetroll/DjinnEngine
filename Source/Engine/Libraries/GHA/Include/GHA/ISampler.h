#pragma once

#include "IResource.h"
#include "SamplerDesc.h"

namespace Djinn {
    namespace GHA {
        class ISampler : public IResource {
        public:
            DJINN_COM_INTERFACE();

            ISampler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IResource(registry, outer, host) {

            }

            // Get description
            virtual const SamplerDesc& GetDesc() = 0;

            /// Overrides
            virtual COM::InterfaceID GetIID() override {
                return kIID;
            }
        };
    }
}