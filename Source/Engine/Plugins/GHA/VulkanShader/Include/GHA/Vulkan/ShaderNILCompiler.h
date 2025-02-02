//
// Created by Dev on 11/29/2017.
//
#pragma once

#include <Data/IShaderNILCompiler.h>
#include <Data/ShaderConfiguration.h>

namespace Djinn::GHA::Vulkan {
    class DJINN_EXPORT_PLUGINS_GHA_VULKANSHADER ShaderNILCompiler : public Data::IShaderNILCompiler {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        ShaderNILCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this compiler
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual COM::Result Compile(NyLang::Registry* registry, const Data::ShaderCompileNILDesc &desc, NyLang::IL::Program *program, GHA::ShaderBlob* blob, ICompiledShaderBlob **out) override;
    };
}