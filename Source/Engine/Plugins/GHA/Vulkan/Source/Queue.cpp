#include <GHA/Vulkan/Queue.h>
#include <Core/Assert.h>
#include <Core/Math.h>
#include <GHA/Vulkan/Fence.h>
#include <GHA/Vulkan/CommandList.h>
#include <GHA/Vulkan/Semaphore.h>
#include <Base/DefaultLogs.h>

DJINN_NS3(GHA, Vulkan);

Queue::Queue(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : COM::IUnknown(registry, outer, host) {

}


COM::Result Queue::Initialize(const vk::QueueFamilyProperties &queueFamilyProperty, UInt32 familyIndex, UInt32 index, const vk::Queue &queue) {
    this->queueFamilyProperties = queueFamilyProperty;
    this->queue = queue;
    this->index = index;
    this->familyIndex = familyIndex;
    // this->swapchain = swapchain;

    // ...
    return COM::kOK;
}

COM::Result Queue::Submit(CommandList *list, Fence *fence, Semaphore *waitSemaphore, Semaphore *signalSemaphore) {
    workGroup.Queue([=] {
        // Wait stage
        vk::PipelineStageFlags stage = vk::PipelineStageFlagBits::eBottomOfPipe;

        // Prepare submission
        vk::SubmitInfo info;
        info.commandBufferCount = 1;
        info.pCommandBuffers = &list->GetBuffer();
        if (waitSemaphore) {
            info.waitSemaphoreCount = 1;
            info.pWaitSemaphores = &(waitSemaphore)->GetSemaphore();
            info.pWaitDstStageMask = &stage;
        }
        if (signalSemaphore) {
            info.signalSemaphoreCount = 1;
            info.pSignalSemaphores = &(signalSemaphore)->GetSemaphore();
        }

        // Submit
        if (vk::CheckedResult result = queue.submit(1, &info, fence ? (fence)->GetFence() : nullptr)) {
            ErrorLog(this).Write("Failed to submit command buffers to queue: ", vk::to_string(result).c_str());
        }
    });
    return COM::kOK;
}

GHA::QueueTypeSet Queue::GetType() {
    GHA::QueueTypeSet set;
    if (queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics) {
        set += GHA::QueueType::eGraphics;
    }
    if (queueFamilyProperties.queueFlags & vk::QueueFlagBits::eCompute) {
        set += GHA::QueueType::eCompute;
    }
    if (queueFamilyProperties.queueFlags & vk::QueueFlagBits::eTransfer) {
        set += GHA::QueueType::eTransfer;
    }
    return set;
}

COM::Result Queue::Wait() {
    queue.waitIdle();
    return COM::kOK;
}

COM::Result Queue::Released() {
    // OK
    return COM::kOK;
}