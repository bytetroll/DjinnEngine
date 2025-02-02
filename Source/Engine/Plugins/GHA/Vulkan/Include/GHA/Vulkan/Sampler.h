//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <GHA/ISampler.h>
#include <vulkan/vulkan.ext.hpp>

namespace Djinn::GHA::Vulkan {
    class DJINN_EXPORT_PLUGINS_GHA_VULKAN Sampler : public ISampler {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Sampler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const SamplerDesc &desc, vk::Device device);

        /// Overrides
        virtual const SamplerDesc &GetDesc() override;

        /// Get sampler
        /// \return
        const vk::Sampler &GetSampler() {
            return sampler;
        }

        /// Get descriptor
        /// \return
        const vk::DescriptorImageInfo &GetDescriptor() {
            return descriptor;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        SamplerDesc desc;
        vk::Device device;
        vk::Sampler sampler;
        vk::DescriptorImageInfo descriptor;
    };
}