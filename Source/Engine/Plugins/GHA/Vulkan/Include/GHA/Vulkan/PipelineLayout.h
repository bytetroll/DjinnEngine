//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include "PipelineLayoutDesc.h"

namespace Djinn::GHA::Vulkan {
    class DJINN_EXPORT_PLUGINS_GHA_VULKAN PipelineLayout : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        PipelineLayout(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const PipelineLayoutDesc &desc, vk::Device device);

        /// Overrides
        const PipelineLayoutDesc &GetDesc();

        /// Get layout
        /// \return
        const vk::PipelineLayout &GetLayout() {
            return pipelineLayout;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        vk::Device device;
        vk::PipelineLayout pipelineLayout;
        PipelineLayoutDesc desc;
    };
}