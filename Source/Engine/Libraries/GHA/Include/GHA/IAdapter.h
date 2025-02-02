#pragma once

#include <COM/IUnknown.h>
#include <Core/String.h>

namespace Djinn {
    namespace GHA {
        class IOutput;
        class ICapabilities;

        class IAdapter : public COM::TUnknown<IAdapter> {
        public:
            DJINN_COM_INTERFACE();

            IAdapter(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

            }

            /// Get outputs
            /// \param count
            /// \param adapters
            /// \return
            virtual COM::Result GetOutputs(UInt32& count, IOutput** adapters) = 0;

            /// Get capabilities
            /// \return
            virtual ICapabilities* GetCapabilities() = 0;

            /// Get decoration
            /// \return
            virtual Core::String GetDecoration() = 0;

            /// Get memory
            /// \return
            virtual USize GetMemory() = 0;
        };
    }
}