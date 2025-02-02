//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <GHA/IShader.h>
#include <vulkan/vulkan.ext.hpp>

namespace Djinn::GHA::Vulkan {
    class CompiledShaderBlob;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN Shader : public IShader {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Shader(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const GHA::ShaderDesc &desc, CompiledShaderBlob *blob, vk::Device device);

        /// Overrides
        const ShaderDesc &GetDesc() override;
        ICompiledShaderBlob *GetBlob() override;

        /// Get module
        /// \return
        const vk::ShaderModule &GetModule() const {
            return module;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        CompiledShaderBlob *blob;
        vk::Device device;
        GHA::ShaderDesc desc;
        vk::ShaderModule module;

    };
}