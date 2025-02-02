//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 11/23/2017.
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