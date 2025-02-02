//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <GHA/ICapabilities.h>
#include <vulkan/vulkan.ext.hpp>

namespace Djinn::GHA::Vulkan {
    class DJINN_EXPORT_PLUGINS_GHA_VULKAN Capabilities : public ICapabilities {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Capabilities(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(vk::PhysicalDevice &physicalDevice);

        /// Overrides
        virtual USize GetFormatSize(GHA::Format format) override;
        virtual bool IsFormatSupported(GHA::Format format, const FormatCapabilitySet &capabilities) override;
        virtual bool SelectFormat(UInt32 count, Format *formats, const FormatCapabilitySet &capabilities, Format &out) override;

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        vk::FormatProperties formatProperties[static_cast<int>(GHA::Format::eCount)];
    };
}