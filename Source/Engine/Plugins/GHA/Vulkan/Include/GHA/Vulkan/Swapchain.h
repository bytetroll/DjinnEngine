//
// Created by Dev on 11/23/2017.
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