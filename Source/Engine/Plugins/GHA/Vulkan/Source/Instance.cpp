#include <GHA/Vulkan/Instance.h>
#include <GHA/Vulkan/Adapter.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <Core/Array.h>
#include <GHA/Vulkan/Adapter.h>
#include <Core/Memory.h>
#include <GHA/Vulkan/Device.h>

DJINN_NS3(GHA, Vulkan);

Instance::Instance(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IInstance(registry, outer, host) {
    // Get layer count
    UInt32 layerCount;
    if (vk::CheckedResult result = vk::enumerateInstanceLayerProperties(&layerCount, nullptr)) {
        ErrorLog(this).Write("Failed to get vulkan layer count: ", vk::to_string(result).c_str());
        return;
    }

    // Get layers
    layerProperties.Resize(layerCount);
    if (vk::CheckedResult result = vk::enumerateInstanceLayerProperties(&layerCount, layerProperties.Ptr())) {
        ErrorLog(this).Write("Failed to get vulkan layers: ", vk::to_string(result).c_str());
        return;
    }

    // Map layers
    layerProperties.Foreach([&](const vk::LayerProperties &property) { layerPropertiesMap[property.layerName] = property; });

    // Get extension count
    UInt32 extensionCount;
    if (vk::CheckedResult result = vk::enumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr)) {
        ErrorLog(this).Write("Failed to get vulkan extension count: ", vk::to_string(result).c_str());
        return;
    }

    // Get extensions
    extensionProperties.Resize(extensionCount);
    if (vk::CheckedResult result = vk::enumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties.Ptr())) {
        ErrorLog(this).Write("Failed to get vulkan extensions: ", vk::to_string(result).c_str());
        return;
    }

    // Map extensions
    extensionProperties.Foreach([&](const vk::ExtensionProperties &property) { extensionPropertiesMap[property.extensionName] = property; });

}

COM::Result Instance::Initialize(const GHA::InstanceDesc &desc) {
    // Core surface extension
    if (!AddExtension("VK_KHR_surface")) {
        ErrorLog(this).Write("Vulkan instance failed to initialize, one of more required extension(s) not supported");
        return COM::kError;
    }

    // Marker extension
    supportsMarkers = AddExtension("VK_EXT_debug_marker");

    // Platform surface extension
#ifdef DJINN_PLATFORM_WINDOWS
    if (!AddExtension("VK_KHR_win32_surface")) {
        return COM::kError;
    }
#else
    if (!AddExtension("VK_KHR_xlib_surface")) {
        return COM::kError;
    }
#endif

    // Debug flag
    if (desc.flags & GHA::InstanceFlag::eDebug) {
        if (!AddExtension("VK_EXT_debug_report") || !AddLayer("VK_LAYER_LUNARG_standard_validation")) {
            return COM::kError;
        }
    }

    // Create application
    vk::ApplicationInfo applicationInfo;
    applicationInfo.pApplicationName = "Djinn.Adapters.Vulkan";
    applicationInfo.pEngineName = "DjinnEngine";
    applicationInfo.apiVersion = VK_MAKE_VERSION(1, 0, 1);
    //applicationInfo.apiVersion = VK_VERSION_1_0;

    // Create instance
    vk::InstanceCreateInfo instanceInfo;
    instanceInfo.pApplicationInfo = &applicationInfo;
    instanceInfo.enabledLayerCount = static_cast<UInt32>(layers.Size());
    instanceInfo.enabledExtensionCount = static_cast<UInt32>(extensions.Size());
    instanceInfo.ppEnabledLayerNames = layers.Ptr();
    instanceInfo.ppEnabledExtensionNames = extensions.Ptr();
    if (vk::CheckedResult result = vk::createInstance(&instanceInfo, nullptr, &instance)) {
        ErrorLog(this).Write("Failed to create vulkan instance: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Diagnostic
    InfoLog(this).Write("Initialized");

    // Marker callbacks
    if (supportsMarkers) {
        if (!(debugCmdBegin = reinterpret_cast<PFN_vkCmdDebugMarkerBeginEXT>(instance.getProcAddr("vkCmdDebugMarkerBeginEXT"))) ||
            !(debugCmdEnd = reinterpret_cast<PFN_vkCmdDebugMarkerEndEXT>(instance.getProcAddr("vkCmdDebugMarkerEndEXT")))) {
            ErrorLog(this).Write("Failed get vulkan marker callback handles");
            return COM::kError;
        }
    }

    // Debug callbacks
    if (desc.flags & GHA::InstanceFlag::eDebug) {
        // Get callbacks
        if (!(createDebugReportCallback = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(instance.getProcAddr("vkCreateDebugReportCallbackEXT"))) ||
            !(destroyDebugCallback = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(instance.getProcAddr("vkDestroyDebugReportCallbackEXT"))) ||
                !(debugReportMessage = reinterpret_cast<PFN_vkDebugReportMessageEXT>(instance.getProcAddr("vkDebugReportMessageEXT")))) {
            ErrorLog(this).Write("Failed get vulkan debug callback handles");
            return COM::kError;
        }

        // Create debug state
        VkDebugReportCallbackCreateInfoEXT debugInfo;
        debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        debugInfo.pNext = nullptr;
        debugInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT /* | VK_DEBUG_REPORT_DEBUG_BIT_EXT*/;
        debugInfo.pfnCallback = static_cast<PFN_vkDebugReportCallbackEXT>(Callback);
        debugInfo.pUserData = this;
        if (vk::CheckedResult result = createDebugReportCallback(instance, &debugInfo, nullptr, &debugReportCallback)) {
            ErrorLog(this).Write("Failed to create vulkan debug callbacks: ", vk::to_string(result).c_str());
            return COM::kError;
        }

        // Diagnostic
        InfoLog(this).Write("Registered vulkan debug callbacks");
    }

    // Get adapter count
    std::uint32_t count;
    if (vk::CheckedResult result = instance.enumeratePhysicalDevices(&count, nullptr)) {
        ErrorLog(this).Write("Failed to enumerate vulkan adapters: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // ...
    physicalDevices.Resize(count);

    // Get adapters
    if (vk::CheckedResult result = instance.enumeratePhysicalDevices(&count, physicalDevices.Ptr())) {
        ErrorLog(this).Write("Failed to enumerate vulkan adapters: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Create adapters
    for (vk::PhysicalDevice &physicalDevice : physicalDevices) {
        Adapter *adapter;
        if (!registry->CreateClass(this, &adapter) || !adapter->Initialize(physicalDevice)) {
            ErrorLog(this).Write("Failed to create vulkan adapter");
            return COM::kError;
        }

        // ...
        adapters.Add(adapter);
    }

    // Diagnostic
    InfoLog(this).Write("Initialized ", count, " vulkan adapter(s)");

    // OK
    return COM::kOK;
}


COM::Result Instance::GetAdapters(UInt32 &count, GHA::IAdapter **adapters) {
    if (adapters) {
        Core::Memory::Copy(adapters, this->adapters.Ptr(), count * sizeof(Adapter *));
    } else {
        count = static_cast<UInt32>(this->adapters.Size());
    }
    return COM::kOK;
}

COM::Result Instance::CreateDevice(COM::IUnknown* outer, GHA::IAdapter *adapter, const GHA::DeviceDesc &desc, GHA::IDevice **out) {
    Device *device;
    if (!registry->CreateClass(outer, &device) || !device->Initialize(desc, adapter, instance)) {
        ErrorLog(this).Write("Failed to create vulkan device");
        return COM::kError;
    }

    *out = device;
    return COM::kOK;
}

COM::Result Instance::AddExtension(const char *name) {
    if (!extensionPropertiesMap.Contains(name)) {
        ErrorLog(this).Write("Vulkan extension '", name, "' not supported");
        return COM::kError;
    }
    extensions.Add(name);
    return COM::kOK;
}

COM::Result Instance::AddLayer(const char *name) {
    if (!layerPropertiesMap.Contains(name)) {
        ErrorLog(this).Write("Vulkan layer '", name, "' not supported");
        return COM::kError;
    }
    layers.Add(name);
    return COM::kOK;
}

VkBool32 Instance::Callback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t srcObject, size_t location, int32_t msgCode, const char *pLayerPrefix, const char *pMsg, void *pUserData) {
    auto self = static_cast<Instance *>(pUserData);
    Core::String header =
            Core::String("Vulkan [") + pLayerPrefix + ", " + Core::String(msgCode) + "] " +
            vk::to_string(static_cast<vk::DebugReportObjectTypeEXT>(objType)).c_str() + "-" + static_cast<Int32>(srcObject) + "-" + static_cast<Int32>(location) + " : ";
    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        ErrorLog(self).Write(header, pMsg);
    } else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
        WarningLog(self).Write(header, pMsg);
    } else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
        WarningLog(self).Write(header, pMsg);
    } else if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
        InfoLog(self).Write(header, pMsg);
    } else if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
        InfoLog(self).Write(header, pMsg);
    } else {
        return 1;
    }
    return 0;
}

COM::Result Instance::Released() {
    instance.destroy(nullptr);

    // OK
    return COM::kOK;
}
