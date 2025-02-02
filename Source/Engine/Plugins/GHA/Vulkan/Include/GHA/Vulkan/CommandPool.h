//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include <GHA/PipelineScheduler/CommandPoolDesc.h>
#include <GHA/PipelineScheduler/CommandListDesc.h>

namespace Djinn::GHA::Vulkan {
    class Queue;
    class CommandList;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN CommandPool : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        CommandPool(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const PipelineScheduler::CommandPoolDesc &desc, Queue *queue, vk::Device device);

        /// Overrides
        const PipelineScheduler::CommandPoolDesc &GetDesc();
        COM::Result CreateCommandList(COM::IUnknown *outer, const PipelineScheduler::CommandListDesc &desc, CommandList **out);

        /// Get pool
        /// \return
        vk::CommandPool &GetPool() {
            return pool;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        Queue *queue;
        PipelineScheduler::CommandPoolDesc desc;
        vk::Device device;
        vk::CommandPool pool;
    };
}