//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include <GHA/PipelineScheduler/FenceDesc.h>
#include <Core/Template/Limits.h>

namespace Djinn::GHA::Vulkan {
    class DJINN_EXPORT_PLUGINS_GHA_VULKAN Fence : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Fence(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize
        /// \return
        COM::Result Initialize(const PipelineScheduler::FenceDesc &desc, vk::Device device);

        /// Overrides
        const PipelineScheduler::FenceDesc &GetDesc();

        /// Is this fence completed?
        bool IsComplete() {
            return device.getFenceStatus(fence) == vk::Result::eSuccess;
        }

        /// Wait for the fence to complete
        void Wait() {
            device.waitForFences(1, &fence, true, Core::LimitsOf<UInt64>::kMax);
        }

        /// Get fence
        /// \return
        const vk::Fence &GetFence() const {
            return fence;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        vk::Fence fence;
        vk::Device device;
        PipelineScheduler::FenceDesc desc;
    };
}