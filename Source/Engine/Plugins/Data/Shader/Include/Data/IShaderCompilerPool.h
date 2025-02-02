#pragma once

#include <COM/IUnknown.h>

namespace NyLang {
    class Registry;
}

namespace Djinn::Data {
    class IShaderCompilerPool : public COM::TUnknown<IShaderCompilerPool> {
    public:
        DJINN_COM_INTERFACE();

        IShaderCompilerPool(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Pool compiler, may wait
        /// \param out
        /// \return
        virtual COM::Result Pool(NyLang::Registry** out) = 0;

        /// Push compiler
        /// \param compiler
        virtual void Push(NyLang::Registry* compiler) = 0;
    };
}