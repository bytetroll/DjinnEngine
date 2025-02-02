#include <GHA/Vulkan/CommandPool.h>
#include <GHA/Vulkan/CommandList.h>
#include <GHA/Vulkan/Queue.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>

DJINN_NS3(GHA, Vulkan);

Vulkan::CommandPool::CommandPool(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUnknown(registry, outer, host) {

}

COM::Result CommandPool::Initialize(const PipelineScheduler::CommandPoolDesc &desc, Queue *queue, vk::Device device) {
    this->desc = desc;
    this->device = device;
    this->queue = queue;

    // Attempt to create queue
    vk::CommandPoolCreateInfo createInfo;
    createInfo.queueFamilyIndex = queue->GetFamilyIndex();
    createInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    if (vk::CheckedResult result = device.createCommandPool(&createInfo, nullptr, &pool)) {
        ErrorLog(this).Write("Failed to create vulkan command pool: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // ...
    return COM::kOK;
}


const PipelineScheduler::CommandPoolDesc &CommandPool::GetDesc() {
    return desc;
}

COM::Result CommandPool::CreateCommandList(COM::IUnknown* outer, const PipelineScheduler::CommandListDesc &desc, CommandList **out) {
    // Create command list
    CommandList *list;
    if (!registry->CreateClass(outer, &list) || !list->Initialize(desc, device, pool)) {
        ErrorLog(this).Write("Failed to create vulkan command list");
        return COM::kError;
    }

    // ...
    *out = list;
    return COM::kOK;
}

COM::Result CommandPool::Released() {
    device.destroyCommandPool(pool, nullptr);

    // OK
    return COM::kOK;
}
