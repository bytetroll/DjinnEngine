#pragma once

#include <GHA/Stage.h>
#include <COM/IUnknown.h>
#include <Data/IAssetCompiler.h>
#include "ShaderConfiguration.h"

namespace NyLang {
    class Registry;
}

namespace Djinn::Data {
    class ShaderAsset;
    class IShaderNILCompiler;
    class IShaderCompilerPool;

    /// Common results
    DJINN_COM_RESULT(NoNILCompiler, true, "Data");

    class DJINN_EXPORT_PLUGINS_DATA_SHADER IShaderCompiler : public IAssetCompiler {
    public:
        DJINN_COM_INTERFACE();

        IShaderCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAssetCompiler(registry, outer, host) {

        }

        /// Compile from stream
        /// \param pool
        /// \param stream
        /// \param config
        /// \param out
        /// \return
        using Djinn::Data::IAssetCompiler::Compile;
        virtual COM::Result Compile(NyLang::Registry* registry, Core::IStream *stream, IAssetConfiguration *config, IAsset **out) = 0;

        /// Add ny intermediate language compiler
        /// \param compiler
        /// \return
        virtual COM::Result AddNILCompiler(IShaderNILCompiler* compiler) = 0;

        /// Add ny intermediate language compiler
        /// \param compiler
        /// \return
        virtual COM::Result RemoveNILCompiler(const COM::ClassID& cid) = 0;

        /// Overrides
        virtual COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case IAssetCompiler::kIID:
                    *out = static_cast<IAssetCompiler*>(this);
                    return COM::kOK;
                case IShaderCompiler::kIID:
                    *out = static_cast<IShaderCompiler*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }
    };
}