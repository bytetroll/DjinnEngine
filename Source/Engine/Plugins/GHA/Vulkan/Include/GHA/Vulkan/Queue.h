//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include <GHA/QueueType.h>
#include <Platform/Thread.h>
#include <Platform/ConditionalVar.h>
#include <Core/Queue.h>
#include <Core/FixedArray.h>
#include <Platform/ConcurrentQueue.h>
#include <Platform/SynchronizedWorkGroup.h>

namespace Djinn::GHA {
    class ISwapchain;
}

namespace Djinn::GHA::Vulkan {
    class CommandList;
    class Fence;
    class Semaphore;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN Queue : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Queue(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const vk::QueueFamilyProperties &queueFamilyProperty, UInt32 familyIndex, UInt32 index, const vk::Queue &queue);

        /// Overrides
        COM::Result Wait();
        QueueTypeSet GetType();
        COM::Result Submit(CommandList *list, Fence *fence, Semaphore *waitSemaphore, Semaphore *signalSemaphore);

        /// Get family properties
        /// \return
        const vk::QueueFamilyProperties &GetFamilyProperties() const {
            return queueFamilyProperties;
        }

        /// Get queue index
        /// \return
        UInt32 GetIndex() const {
            return index;
        }

        /// Get queue family index
        /// \return
        UInt32 GetFamilyIndex() const {
            return familyIndex;
        }

        /// Get queue
        /// \return
        const vk::Queue &GetQueue() const {
            return queue;
        }

        /// Work group
        Platform::SynchronizedWorkGroup workGroup;

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        vk::QueueFamilyProperties queueFamilyProperties;
        UInt32 index;
        UInt32 familyIndex;
        vk::Queue queue;
        ISwapchain *swapchain = nullptr;
    };
}