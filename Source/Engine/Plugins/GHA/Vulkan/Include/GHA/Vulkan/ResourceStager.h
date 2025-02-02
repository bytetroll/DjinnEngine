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
 on 12/13/2017.
//
#pragma once

#include <GHA/IResourceStager.h>
#include <Core/Array.h>
#include <vulkan/vulkan.ext.hpp>
#include <Core/Pool.h>
#include <Core/Map.h>
#include <Core/Queue.h>
#include <Core/Literals.h>
#include <Platform/ConcurrentQueue.h>
#include <Async/Async.h>
#include <Async/Event.h>
#include <Platform/Event.h>
#include <Platform/Thread.h>
#include <Core/FixedArray.h>

namespace Djinn::GHA::Vulkan {
    class Allocator;
    struct Allocation;
    class Queue;

    class ResourceStager : public IResourceStager {
    public:
        DJINN_COM_CLASS();

        ResourceStager(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize this stager
        /// \param device
        /// \return
        COM::Result Initialize(vk::Device device, Allocator *allocator, Queue *queue, USize stageSize = 256_mb);

        /// Overrides
        virtual void StageBuffer(IMemoryResource *resource, void *buffer, const GHA::MemoryBounds &bounds) override;
        virtual void StageTextureMip(ITexture *texture, UInt32 mip, void *buffer, const GHA::MemoryBounds &bounds) override;
        virtual void Wait() override;

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        void _Thread();

        struct PendingStage {
            IMemoryResource *resource;
            void *buffer;
            GHA::MemoryBounds bounds;
            UInt32 mipIndex = 0;
        };

        Core::AtomicCounter jobCounter{0};
        Platform::Mutex pendingMtx;
        Platform::ConditionalVar pendingVar;
        Platform::Thread thread;
        Platform::ConcurrentQueue<PendingStage> pendingStages;
        Platform::ConditionalVar wakeVar;

    private:
        vk::CommandPool pool;
        vk::CommandBuffer commandBuffer;
        Allocator *allocator;
        Allocation *stageAlloc;
        vk::Buffer stageBuffer;
        vk::Device device;
        Queue *queue;
        vk::Fence fence;
        USize offset = 0;
        Core::Atomic<bool> exitFlag{false};
    };
}