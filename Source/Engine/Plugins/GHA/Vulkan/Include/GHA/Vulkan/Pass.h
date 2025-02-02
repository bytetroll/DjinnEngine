//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include <GHA/PipelineScheduler/PassDesc.h>

namespace Djinn::GHA::Vulkan {
    class DJINN_EXPORT_PLUGINS_GHA_VULKAN Pass : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Pass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const PipelineScheduler::PassDesc &desc, vk::Device device);

        /// Overrides
        const PipelineScheduler::PassDesc &GetDesc();

        /// Get render pass
        /// \return
        const vk::RenderPass &GetPass() const {
            return pass;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        vk::RenderPass pass;
        vk::Device device;
        PipelineScheduler::PassDesc desc;
    };
}