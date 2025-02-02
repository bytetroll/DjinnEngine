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

#include <GHA/ISwapchain.h>
#include <vulkan/vulkan.ext.hpp>
#include <Core/Array.h>
#include <Core/Pool.h>
#include <Platform/Event.h>

namespace Djinn::GHA {
    class IAdapter;
}

namespace Djinn::GHA::Vulkan {
    class Semaphore;

    class Fence;

    class Queue;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN Swapchain : public ISwapchain {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Swapchain(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const SwapchainDesc &desc, IAdapter *adapter, vk::Device device, vk::Instance &instance);

        /// Overrides
        const SwapchainDesc &GetDesc() override;
        UInt32 GetCurrentIndex() override;
        ITexture *GetTexture(int index) override;
        Semaphore *GetBackbufferSemaphore();
        ITexture *GetTexture() override;
        void Begin(Queue *queue);
        void Present(Queue *queue, Semaphore *waitSemaphore);
        Fence *GetBackbufferFence();

        /// Get swapchain
        /// \return
        const vk::SwapchainKHR &GetSwapchain() const {
            return swapchain;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        COM::Result ReleaseResources();

        COM::Result Reinitialize();

        IAdapter *adapter;
        vk::Instance instance;
        vk::SwapchainKHR swapchain;
        GHA::SwapchainDesc desc;
        vk::SurfaceKHR surface;
        vk::SurfaceCapabilitiesKHR capabilities;
        vk::Device device;
        Core::AtomicCounter frameCounter{0};
        Core::Array<vk::SurfaceFormatKHR> surfaceFormats;
        Core::Array<vk::PresentModeKHR> presentModes;
        Core::Array<ITexture *> backBuffers;
        UInt32 backBufferIndex = 0;
        Platform::Event queueEvent;
        Core::ObjectPool<Semaphore> semaphorePool;

        struct Frame {
            bool submitted = false;
            Semaphore *semaphore = nullptr;
            Fence *fence = nullptr;
        };

        Core::Array<Frame> frames;
    };
}