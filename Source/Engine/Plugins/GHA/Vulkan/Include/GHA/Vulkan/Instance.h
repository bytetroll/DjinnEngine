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

#include <GHA/IInstance.h>
#include <vulkan/vulkan.ext.hpp>
#include <Core/Array.h>
#include <Core/HashMap.h>
#include <Core/String.h>

namespace Djinn::GHA::Vulkan {
    class Adapter;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN Instance : public IInstance {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Instance(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Overrides
        virtual COM::Result Initialize(const InstanceDesc &desc) override;
        virtual COM::Result GetAdapters(UInt32 &count, IAdapter **adapters) override;
        virtual COM::Result CreateDevice(COM::IUnknown *outer, GHA::IAdapter *adapter, const DeviceDesc &desc, IDevice **out) override;

        /// Add extension
        /// \param name
        COM::Result AddExtension(const char *name);

        /// Add layer
        /// \param name
        COM::Result AddLayer(const char *name);

        PFN_vkCmdDebugMarkerBeginEXT GetDebugCmdBegin() const {
            return debugCmdBegin;
        }

        PFN_vkCmdDebugMarkerEndEXT GetDebugCmdEnd() const {
            return debugCmdEnd;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        vk::Instance instance;
        Core::Array<Adapter *> adapters;
        Core::Array<vk::PhysicalDevice> physicalDevices;
        Core::Array<const char *> extensions;
        Core::Array<const char *> layers;
        Core::Array<vk::ExtensionProperties> extensionProperties;
        Core::Array<vk::LayerProperties> layerProperties;
        Core::HashMap<Core::String, vk::ExtensionProperties> extensionPropertiesMap;
        Core::HashMap<Core::String, vk::LayerProperties> layerPropertiesMap;

    private:
        /// Vulkan debug callback
        /// \return
        static VkBool32 Callback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t srcObject, size_t location, int32_t msgCode, const char *pLayerPrefix, const char *pMsg, void *pUserData);

        // Callback handles
        bool supportsMarkers = false;
        VkDebugReportCallbackEXT debugReportCallback = nullptr;
        PFN_vkCreateDebugReportCallbackEXT createDebugReportCallback = nullptr;
        PFN_vkDestroyDebugReportCallbackEXT destroyDebugCallback = nullptr;
        PFN_vkDebugReportMessageEXT debugReportMessage = nullptr;
        PFN_vkCmdDebugMarkerBeginEXT debugCmdBegin = nullptr;
        PFN_vkCmdDebugMarkerEndEXT debugCmdEnd = nullptr;
    };
}