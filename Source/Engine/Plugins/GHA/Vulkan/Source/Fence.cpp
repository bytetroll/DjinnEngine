#include <GHA/Vulkan/Fence.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <GHA/PipelineScheduler/FenceDesc.h>

DJINN_NS3(GHA, Vulkan);

Fence::Fence(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : COM::IUnknown(registry, outer, host) {

}

COM::Result Fence::Initialize(const PipelineScheduler::FenceDesc &desc, vk::Device device) {
    this->device = device;
    this->desc = desc;

    // Attempt to create fence
    vk::FenceCreateInfo info;
    info.flags = {};
    if (vk::CheckedResult result = device.createFence(&info, nullptr, &fence)) {
        ErrorLog(this).Write("Failed to create vulkan fence: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

const PipelineScheduler::FenceDesc &Fence::GetDesc() {
    return desc;
}

COM::Result Fence::Released() {
    device.destroyFence(fence, nullptr);

    // OK
    return COM::kOK;
}
