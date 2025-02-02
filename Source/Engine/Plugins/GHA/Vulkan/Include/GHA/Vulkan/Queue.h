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