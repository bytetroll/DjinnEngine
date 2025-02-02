#include <GHA/Vulkan/Swapchain.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <GHA/Vulkan/Adapter.h>
#include <GHA/Vulkan/EnumUtils.h>
#include <GHA/Vulkan/Texture.h>
#include <GHA/Vulkan/TextureView.h>
#include <GHA/Vulkan/Semaphore.h>
#include <GHA/Vulkan/Queue.h>
#include <GHA/Vulkan/Fence.h>

DJINN_NS3(GHA, Vulkan);

Swapchain::Swapchain(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISwapchain(registry, outer, host) {

}


const GHA::SwapchainDesc &Swapchain::GetDesc() {
    return desc;
}

COM::Result Swapchain::Initialize(const GHA::SwapchainDesc &desc, IAdapter *adapter, vk::Device device, vk::Instance &instance) {
    this->desc = desc;
    this->device = device;
    this->adapter = adapter;
    this->instance = instance;

    vk::PhysicalDevice physicalDevice = static_cast<Adapter *>(adapter)->GetPhysicalDevice();

#ifdef DJINN_PLATFORM_WINDOWS
    vk::Win32SurfaceCreateInfoKHR surfaceInfo;
    surfaceInfo.hinstance = desc.windowHandle.hInstance;
    surfaceInfo.hwnd = desc.windowHandle.hWnd;
    if (vk::CheckedResult result = instance.createWin32SurfaceKHR(&surfaceInfo, nullptr, &surface)) {
        ErrorLog(this).Write("Failed to create vulkan win32 surface: ", vk::to_string(result).c_str());
        return COM::kError;
    }
#else
    vk::XlibSurfaceCreateInfoKHR surfaceInfo;
    surfaceInfo.window = desc.windowHandle.window;
    surfaceInfo.dpy = reinterpret_cast<Display*>(desc.windowHandle.display);
    if (vk::CheckedResult result = instance.createXlibSurfaceKHR(&surfaceInfo, nullptr, &surface)) {
        ErrorLog(this).Write("Failed to create vulkan linux surface: ", vk::to_string(result).c_str());
        return COM::kError;
    }
#endif

    // Get surface support
    UInt32 surfaceSupported;
    if (vk::CheckedResult result = physicalDevice.getSurfaceSupportKHR(0, surface, &surfaceSupported)) {
        ErrorLog(this).Write("Failed to get vulkan surface support: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Must be supported
    if (!surfaceSupported) {
        ErrorLog(this).Write("Platform vulkan surface not supported");
        return COM::kError;
    }

    // Get surface capabilities
    if (vk::CheckedResult result = physicalDevice.getSurfaceCapabilitiesKHR(surface, &capabilities)) {
        ErrorLog(this).Write("Failed to get vulkan surface capabilities: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Enumerate present modes
    UInt32 presentCount;
    if (vk::CheckedResult result = physicalDevice.getSurfacePresentModesKHR(surface, &presentCount, nullptr)) {
        ErrorLog(this).Write("Failed to get vulkan surface present modes: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Enumerate present modes
    presentModes.Resize(presentCount);
    if (vk::CheckedResult result = physicalDevice.getSurfacePresentModesKHR(surface, &presentCount, presentModes.Ptr())) {
        ErrorLog(this).Write("Failed to get vulkan surface present modes: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Determine present mode
    vk::PresentModeKHR presentMode = {};
    switch (desc.swapMode) {
        case SwapMode::eImmediate:
            presentMode = vk::PresentModeKHR::eImmediate;
            break;
        case SwapMode::eMailbox:
            presentMode = vk::PresentModeKHR::eMailbox;
            break;
        case SwapMode::eFIFO:
            presentMode = vk::PresentModeKHR::eFifo;
            break;
        case SwapMode::eFIFORelaxed:
            presentMode = vk::PresentModeKHR::eFifoRelaxed;
            break;
    }

    // Present mode must be supported
    if (!presentModes.Any([&](vk::PresentModeKHR mode) {
        return mode == presentMode;
    })) {
        ErrorLog(this).Write("Swapmode not supported");
        return COM::kError;
    }

    // Determine surface size
    vk::Extent2D surfaceExtent = capabilities.currentExtent;
    if (capabilities.currentExtent.width == 0) {
        surfaceExtent = vk::Extent2D(desc.dimensions->width, desc.dimensions->height);
    }

    // Determine number of backbuffers
    UInt32 backbufferCount;
    if (desc.backbufferCount.IsValid()) {
        backbufferCount = desc.backbufferCount.Get();
        if (backbufferCount < capabilities.minImageCount || backbufferCount > capabilities.maxImageCount) {
            ErrorLog(this).Write("Backbuffer count '", desc.backbufferCount.Get(), "' not supported");
            return COM::kError;
        }
    } else {
        //backbufferCount = capabilities.maxImageCount;
        backbufferCount = Math::Min(3U, Math::Max(capabilities.minImageCount, capabilities.maxImageCount));
    }

    // ...
    this->desc.backbufferCount = backbufferCount;

    // No backbuffers?
    if (backbufferCount == 0) {
        ErrorLog(this).Write("No valid backbuffers");
        return COM::kError;
    }

    // Determine surface transforms
    vk::SurfaceTransformFlagBitsKHR transform;
    if (capabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) {
        transform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
    }

    // Enumerate formats
    UInt32 formatCount;
    if (vk::CheckedResult result = physicalDevice.getSurfaceFormatsKHR(surface, &formatCount, nullptr)) {
        ErrorLog(this).Write("Failed to get vulkan surface formats: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Enumerate formats
    surfaceFormats.Resize(formatCount);
    if (vk::CheckedResult result = physicalDevice.getSurfaceFormatsKHR(surface, &formatCount, surfaceFormats.Ptr())) {
        ErrorLog(this).Write("Failed to get vulkan surface formats: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Determine format
    vk::SurfaceFormatKHR format;
    if (desc.format.IsValid()) {
        // Must exist
        UInt32 index = 0;
        if (!surfaceFormats.Any([&](const vk::SurfaceFormatKHR &surfaceFormat) {
            index++;
            return surfaceFormat.format == AsFormat(desc.format.Get());
        })) {
            ErrorLog(this).Write("Requested format not supported");
            return COM::kError;
        }

        // ...
        format = surfaceFormats[index];
    } else {
        format = surfaceFormats[0];
    }

    // Create Swapchain
    vk::SwapchainCreateInfoKHR info;
    info.surface = surface;
    info.clipped = static_cast<vk::Bool32>(true);
    info.minImageCount = backbufferCount;
    info.imageFormat = format.format;
    info.imageColorSpace = format.colorSpace;
    info.imageExtent = surfaceExtent;
    info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst;
    info.preTransform = transform;
    info.imageArrayLayers = 1;
    info.queueFamilyIndexCount = 0;
    info.pQueueFamilyIndices = nullptr;
    info.presentMode = presentMode;
    info.oldSwapchain = nullptr;
    info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    if (vk::CheckedResult result = device.createSwapchainKHR(&info, nullptr, &swapchain)) {
        ErrorLog(this).Write("Failed to create vulkan swapchain: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Enumerate backbuffers
    if (vk::CheckedResult result = device.getSwapchainImagesKHR(swapchain, &backbufferCount, nullptr)) {
        ErrorLog(this).Write("Failed to get vulkan surface backbuffers: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Enumerate backbuffers
    Core::Array<vk::Image> backbufferImages(backbufferCount);
    if (vk::CheckedResult result = device.getSwapchainImagesKHR(swapchain, &backbufferCount, backbufferImages.Ptr())) {
        ErrorLog(this).Write("Failed to get vulkan surface backbuffers: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Create textures
    backBuffers.Resize(backbufferCount);
    for (USize i = 0; i < backbufferCount; i++) {
        Texture* texture;
        Check(registry->CreateClass(this, &texture));

        // Initialize texture
        if (!texture->Initialize(
                TextureDesc()
                        .Format(AsGHAFormat(format.format))
                        .Size(Size(surfaceExtent.width, surfaceExtent.height))
                        .Flags(TextureFlag::eClearable)
                        .Usage(TextureUsage::eColor)
                        .Type(TextureType::e2D),
                backbufferImages[i],
                device
        )) {
            ErrorLog(this).Write("Failed to create vulkan backbuffer texture wrapper");
            return COM::kError;
        }

        // ...
        backBuffers[i] = texture;
    }

    // Create image semaphores and image frames
    frames.Resize(backbufferCount);
    for (UInt32 i = 0; i < backbufferCount; i++) {
        // Create semaphore
        Semaphore* semaphore;
        if (!registry->CreateClass(nullptr, &semaphore) || !semaphore->Initialize(device)) {
            ErrorLog(this).Write("Failed to create vulkan backbuffer image semaphore");
            return COM::kError;
        }

        // Push
        semaphorePool.Push(semaphore);

        // Create fence
        if (!registry->CreateClass(nullptr, &frames[i].fence) || !frames[i].fence->Initialize(PipelineScheduler::FenceDesc(), device)) {
            ErrorLog(this).Write("Failed to create vulkan backbuffer image fence");
            return COM::kError;
        }
    }

    // Get next index and signal image semaphore for inbound command buffers
    frames[0].semaphore = semaphorePool.PopOrNull();
    if (vk::CheckedResult result = device.acquireNextImageKHR(swapchain, Core::LimitsOf<UInt64>::kMax, frames[0].semaphore->GetSemaphore(), nullptr, &backBufferIndex)) {
        ErrorLog(this).Write("Failed to get next vulkan backbuffer image index: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

void Swapchain::Present(Queue *queue, Semaphore *waitSemaphore) {
    queue->workGroup.Queue([&] {
        auto& frame = frames[backBufferIndex];

        // Describe present
        vk::PresentInfoKHR info;
        info.swapchainCount = 1;
        info.pSwapchains = &swapchain;
        info.pImageIndices = &backBufferIndex;
        info.pResults = nullptr;
        if (waitSemaphore) {
            info.waitSemaphoreCount = 1;
            info.pWaitSemaphores = &(waitSemaphore)->GetSemaphore();
        }

        // Present in target queue
        if (vk::CheckedResult result = (queue)->GetQueue().presentKHR(&info)) {
            if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR) {
                // Wait for all current work
                Sink(queue->Wait());

                // ...
                Sink(Reinitialize());
                return;
            } else {
                ErrorLog(this).Write("Failed to present vulkan swapchain: ", vk::to_string(result).c_str());
                return;
            }
        }

        // Swap semaphores
        semaphorePool.Push(frame.semaphore);
        frame.semaphore = nullptr;
        auto semaphore = semaphorePool.PopOrNull();

        // Get next index and signal image semaphore for inbound command buffers
        if (vk::CheckedResult result = device.acquireNextImageKHR(swapchain, Core::LimitsOf<UInt64>::kMax, semaphore->GetSemaphore(), nullptr, &backBufferIndex)) {
            if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR) {
                // Wait for all current work
                Sink(queue->Wait());

                // ...
                Sink(Reinitialize());
                return;
            } else {
                ErrorLog(this).Write("Failed to get next vulkan backbuffer image index: ", vk::to_string(result).c_str());
            }
        }

        // Configure next frame
        frames[backBufferIndex].semaphore = semaphore;

        // Increment
        frameCounter++;
    }, &queueEvent);

    // Wait for the present
    queueEvent.Wait();
}

void Swapchain::Begin(Queue *queue) {
    auto& frame = frames[backBufferIndex];

    // Ignore first frames
    if (frame.submitted) {
        // Wait for fence
        if (vk::CheckedResult result = device.waitForFences(1, &frame.fence->GetFence(), static_cast<vk::Bool32>(true), Core::LimitsOf<USize>::kMax)) {
            if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR) {
                // Wait for all current work
                Sink(queue->Wait());

                // ...
                Sink(Reinitialize());
                return;
            } else {
                ErrorLog(this).Write("Failed to wait for vulkan backbuffer fence: ", vk::to_string(result).c_str());
                return;
            }
        }

        // Reset
        if (vk::CheckedResult result = device.resetFences(1, &frame.fence->GetFence())) {
            if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR) {
                // Wait for all current work
                Sink(queue->Wait());

                // ...
                Sink(Reinitialize());
                return;
            } else {
                ErrorLog(this).Write("Failed to reset vulkan backbuffer fence: ", vk::to_string(result).c_str());
                return;
            }
        }
    }

    // Mark as submitted
    frame.submitted = true;
}

UInt32 Swapchain::GetCurrentIndex() {
    return backBufferIndex;
}

GHA::ITexture *Swapchain::GetTexture(int index) {
    return backBuffers[index];
}

Semaphore *Swapchain::GetBackbufferSemaphore() {
    return frames[backBufferIndex].semaphore;
}

Fence *Swapchain::GetBackbufferFence() {
    return frames[backBufferIndex].fence;
}

ITexture *Swapchain::GetTexture() {
    return backBuffers[backBufferIndex];
}

COM::Result Swapchain::Reinitialize() {
    InfoLog(this).Write("Reinitializing swapchain to match out of date window handle");

    // Release all resources
    if (COM::CheckedResult result = ReleaseResources()) {
        return result;
    }

    // Initialize
    if (COM::CheckedResult result = Initialize(desc, adapter, device, instance)) {
        return result;
    }

    // OK
    return COM::kOK;
}

COM::Result Swapchain::ReleaseResources() {
    // Destroy semaphores
    Semaphore* sem;
    while ((sem = semaphorePool.PopOrNull())) {
        Check(sem->Release());
    }

    // Destroy fences
    for (auto& frame : frames) {
        if (frame.semaphore) {
            Check(frame.semaphore->Release());
        }
        Check(frame.fence->Release());
    }

    backBuffers.Clear();
    presentModes.Clear();
    surfaceFormats.Clear();
    frames.Clear();
    device.destroySwapchainKHR(swapchain, nullptr);
    instance.destroySurfaceKHR(surface, nullptr);
    return COM::kOK;
}

COM::Result Swapchain::Released() {
    Check(ReleaseResources());

    // OK
    return COM::kOK;
}
