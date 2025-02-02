//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include <GHA/PipelineScheduler/PipelineDesc.h>
#include <GHA/Stage.h>
#include <GHA/BindPoint.h>

namespace Djinn::GHA {
    class ICapabilities;
}

namespace Djinn::GHA::Vulkan {
    class Pass;
    class PipelineLayout;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN Pipeline : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Pipeline(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const PipelineScheduler::PipelineDesc &desc, PipelineLayout *layout, Pass *pass, vk::PipelineCache &cache, ICapabilities *caps, vk::Device device);

        /// Overrides
        PipelineScheduler::PipelineBinding GetBind(UInt64 hashCrc64, StageSet stageMask);
        const PipelineScheduler::PipelineDesc &GetDesc();
        BindPoint GetBindPoint();

        /// Get pipeline
        /// \return
        const vk::Pipeline &GetPipeline() const {
            return pipeline;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        vk::Pipeline pipeline;
        vk::Device device;
        BindPoint bindPoint;
        PipelineScheduler::PipelineDesc desc;
    };
}