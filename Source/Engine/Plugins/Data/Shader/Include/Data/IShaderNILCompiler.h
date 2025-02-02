#pragma once

#include <COM/IUnknown.h>
#include <Core/Array.h>
#include <NyLang/Core/Host/Registry.h>
#include "ShaderCompilerNILDesc.h"

namespace NyLang::IL {
    class Program;
}

namespace Djinn::GHA {
    class ICompiledShaderBlob;
    struct ShaderBlob;
}

namespace Djinn {
    namespace Data {
        class IShaderNILCompiler : public COM::TUnknown<IShaderNILCompiler> {
        public:
            DJINN_COM_INTERFACE();

            /// Construct
            /// \param registry
            /// \param outer
            IShaderNILCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

            }

            /// Compile nil program into a shader blob
            /// \param program
            /// \param out
            /// \return
            virtual COM::Result Compile(NyLang::Registry* registry, const ShaderCompileNILDesc& desc, NyLang::IL::Program* program, GHA::ShaderBlob* blob, GHA::ICompiledShaderBlob** out) = 0;
        };
    }
}