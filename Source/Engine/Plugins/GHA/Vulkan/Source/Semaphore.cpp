#include <GHA/Vulkan/Semaphore.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>

DJINN_NS3(GHA, Vulkan);

Semaphore::Semaphore(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : COM::IUnknown(registry, outer, host) {

}

COM::Result Semaphore::Initialize(vk::Device device) {
    this->device = device;

    // Attempt to create Semaphore
    vk::SemaphoreCreateInfo info;
    info.flags = {};
    if (vk::CheckedResult result = device.createSemaphore(&info, nullptr, &semaphore)) {
        ErrorLog(this).Write("Failed to create vulkan semaphore: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result Semaphore::Released() {
    device.destroySemaphore(semaphore, nullptr);

    // OK
    return COM::kOK;
}


