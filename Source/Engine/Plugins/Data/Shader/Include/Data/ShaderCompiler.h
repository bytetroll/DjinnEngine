#pragma once

#include <Core/HashMap.h>
#include <NyLang/Core/Shared.h>
#include <GHA/InputAssemblyDesc.h>
#include <Data/NyShader/AnnotationChain.h>
#include <Platform/Mutex.h>
#include "IShaderCompiler.h"

namespace NyLang {
    class Compiler;
    class Diagnostic;
    namespace IL {
        class IValue;
    }
}

namespace Djinn::Data {
    class ShaderCompilerPool;

    class DJINN_EXPORT_PLUGINS_DATA_SHADER ShaderCompiler : public IShaderCompiler {
    public:
        DJINN_COM_CLASS();

        ShaderCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this shader compiler
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual COM::Result Compile(NyLang::Registry* registry, Core::IStream *stream, IAssetConfiguration *config, IAsset **out) override;
        virtual COM::Result Compile(Core::IStream *stream, IAssetConfiguration *config, IAsset **out) override;
        virtual COM::Result AddNILCompiler(IShaderNILCompiler *compiler) override;
        virtual COM::Result RemoveNILCompiler(const COM::ClassID& cid) override;

    private:
        COM::Result FillVertexAttributes(GHA::InputAssemblyDesc& desc, NyLang::IL::IValue* value, NyShader::AnnotationChain chain = {});

        Platform::Mutex mutex;
        ShaderCompilerPool* defaultPool;
        Core::HashMap<COM::ClassID, IShaderNILCompiler*> compilers;
    };
}