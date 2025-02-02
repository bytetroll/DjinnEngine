#include <GHA/Vulkan/Output.h>
#include <vulkan/vulkan.ext.hpp>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>

DJINN_NS3(GHA, Vulkan);

Output::Output(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IOutput(registry, outer, host) {

}

COM::Result Output::Initialize(UInt32 /*queueIndex*/, vk::QueueFamilyProperties& queueFamilyProperties, vk::PhysicalDevice& /*physicalDevice*/) {
    this->queueFamilyProperties = queueFamilyProperties;

    // Diagnostic
    // registry->Log<Base::InfoLog>() << "Created vulkan output '" << displayProperties.displayName << "'\n";

    /*physicalDevice.getDisplayPlaneCapabilitiesKHR(displayModeProperties[0].)
    vk::DisplayModeKHR displayMode;
    physicalDevice.getDisplayPlaneCapabilitiesKHR()

    // Get display plane capabilities count
    UInt32 displayPlaneCapabilitiesCount;
    if (vk::CheckedResult result = physicalDevice.getDisplayPlaneCapabilitiesKHR(&displayPlaneCapabilitiesCount, nullptr)) {
        ErrorLog(this) << "Failed to get vulkan adapter display capabilities properties: " << vk::to_string(result).c_str() << "\n";
        return COM::kError;
    }

    // Get display plane properties
    displayPlaneCapabilities.Resize(displayPlaneCapabilitiesCount);
    if (vk::CheckedResult result = physicalDevice.getDisplayPlaneCapabilitiesKHR(&displayPlaneCapabilitiesCount, displayPlaneCapabilities.Ptr())) {
        ErrorLog(this) << "Failed to get vulkan adapter display capabilities properties: " << vk::to_string(result).c_str() << "\n";
        return COM::kError;
    }*/

    // OK
    return COM::kOK;
}

const GHA::OutputDesc &Output::GetDesc() {
    return desc;
}

COM::Result Output::Released() {
    // OK
    return COM::kOK;
}
