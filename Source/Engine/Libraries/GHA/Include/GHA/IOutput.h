#pragma once

#include <COM/IUnknown.h>
#include "OutputDesc.h"

namespace Djinn {
    namespace GHA {
        class IOutput : public COM::TUnknown<IOutput> {
        public:
            DJINN_COM_INTERFACE();

            /// Construct
            /// \param registry
            /// \param outer
            IOutput(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

            }

            /// Get output description
            /// \return
            virtual const OutputDesc& GetDesc() = 0;
        };
    }
}