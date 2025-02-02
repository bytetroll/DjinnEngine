//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include <GHA/PipelineScheduler/DescriptorSetLayoutDesc.h>

namespace Djinn::GHA::Vulkan {
    class DJINN_EXPORT_PLUGINS_GHA_VULKAN DescriptorSetLayout : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        DescriptorSetLayout(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const PipelineScheduler::DescriptorSetLayoutDesc &desc, vk::Device device);

        /// Overrides
        const PipelineScheduler::DescriptorSetLayoutDesc &GetDesc();

        /// Get layout
        /// \return
        const vk::DescriptorSetLayout &GetLayout() const {
            return layout;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        PipelineScheduler::DescriptorSetLayoutDesc desc;
        vk::Device device;
        vk::DescriptorSetLayout layout;
    };
}