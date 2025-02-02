//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <GHA/IOutput.h>
#include <GHA/Format.h>
#include <Core/Array.h>
#include <vulkan/vulkan.ext.hpp>

namespace Djinn::GHA::Vulkan {
    class DJINN_EXPORT_PLUGINS_GHA_VULKAN Output : public IOutput {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Output(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(UInt32 queueIndex, vk::QueueFamilyProperties &queueFamilyProperties, vk::PhysicalDevice &device);

        /// Overrides
        virtual const OutputDesc &GetDesc() override;

        /// Get available formats
        /// \return
        const Core::Array<GHA::Format> &GetFormats() {
            return formats;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        GHA::OutputDesc desc;
        Core::Array<GHA::Format> formats;
        vk::QueueFamilyProperties queueFamilyProperties;
    };
}