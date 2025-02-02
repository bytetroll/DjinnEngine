#pragma once

#include <COM/IUnknown.h>
#include "ShaderDesc.h"

namespace Djinn {
    namespace GHA {
        class ICompiledShaderBlob;

        class IShader : public COM::TUnknown<IShader> {
        public:
            DJINN_COM_INTERFACE();

            IShader(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

            }

            /// Get description
            /// \return
            virtual const GHA::ShaderDesc& GetDesc() = 0;

            /// Get blob
            /// \return
            virtual ICompiledShaderBlob* GetBlob() = 0;
        };
    }
}