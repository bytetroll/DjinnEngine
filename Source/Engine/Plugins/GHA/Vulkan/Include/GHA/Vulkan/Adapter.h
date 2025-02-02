//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <GHA/IAdapter.h>
#include <Core/String.h>
#include <vulkan/vulkan.ext.hpp>
#include <Core/Array.h>

namespace Djinn::GHA {
    class IOutput;
}

namespace Djinn::GHA::Vulkan {
    class Capabilities;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN Adapter : public IAdapter {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Adapter(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(vk::PhysicalDevice &physicalDevice);

        /// Overrides
        COM::Result GetOutputs(UInt32 &count, IOutput **adapters) override;
        Core::String GetDecoration() override;
        USize GetMemory() override;
        ICapabilities *GetCapabilities() override;

        /// Get physical device
        /// \return
        vk::PhysicalDevice &GetPhysicalDevice() {
            return physicalDevice;
        }

        /// Get memory properties
        /// \return
        const vk::PhysicalDeviceMemoryProperties &GetMemoryProperties() const {
            return memoryProperties;
        }

        /// Get properties
        /// \return
        const vk::PhysicalDeviceProperties &GetProperties() const {
            return properties;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        Capabilities *capabilities;
        vk::PhysicalDevice physicalDevice;
        vk::PhysicalDeviceMemoryProperties memoryProperties;
        vk::PhysicalDeviceProperties properties;
        Core::Array<vk::DisplayPropertiesKHR> displayProperties;
        Core::Array<vk::DisplayPlaneCapabilitiesKHR> displayPlaneCapabilities;
        Core::Array<vk::DisplayPlanePropertiesKHR> displayPlaneProperties;
        Core::Array<GHA::IOutput *> outputs;
        Core::String decoration;
    };
}