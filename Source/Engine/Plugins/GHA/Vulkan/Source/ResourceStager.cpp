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

#include <GHA/Vulkan/ResourceStager.h>
#include <GHA/Vulkan/CommandList.h>
#include <GHA/IMemoryResource.h>
#include <GHA/Vulkan/Texture.h>
#include <GHA/Vulkan/Allocator.h>
#include <GHA/Vulkan/TextureBuffer.h>
#include <GHA/Vulkan/VertexBuffer.h>
#include <GHA/Vulkan/IndexBuffer.h>
#include <GHA/Vulkan/StructuredBuffer.h>
#include <Core/Memory.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <GHA/Vulkan/EnumUtils.h>
#include <GHA/Vulkan/Queue.h>
#include <GHA/Vulkan/Fence.h>

DJINN_NS3(GHA, Vulkan);

ResourceStager::ResourceStager(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IResourceStager(registry, outer, host) {
}

COM::Result ResourceStager::Initialize(vk::Device device, Allocator *allocator, Queue *queue, USize stageSize) {
    this->device = device;
    this->queue = queue;
    this->allocator = allocator;

    // Attempt to create fence
    vk::FenceCreateInfo fenceInfo;
    if (vk::CheckedResult result = device.createFence(&fenceInfo, nullptr, &fence)) {
        ErrorLog(this).Write("Failed to create vulkan fence: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Attempt to create pool
    vk::CommandPoolCreateInfo createInfo;
    createInfo.queueFamilyIndex = queue->GetFamilyIndex();
    createInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    if (vk::CheckedResult result = device.createCommandPool(&createInfo, nullptr, &pool)) {
        ErrorLog(this).Write("Failed to create vulkan command pool: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Attempt to allocate command buffer
    vk::CommandBufferAllocateInfo allocateInfo;
    allocateInfo.commandPool = pool;
    allocateInfo.commandBufferCount = 1;
    allocateInfo.level = vk::CommandBufferLevel::ePrimary;
    if (vk::CheckedResult result = device.allocateCommandBuffers(&allocateInfo, &commandBuffer)) {
        ErrorLog(this).Write("Failed to allocate vulkan command buffer: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Attempt to create buffer
    vk::BufferCreateInfo info;
    info.flags = {};
    info.size = stageSize;
    info.sharingMode = vk::SharingMode::eExclusive;
    info.usage = vk::BufferUsageFlagBits::eTransferSrc;
    if (vk::CheckedResult result = device.createBuffer(&info, nullptr, &stageBuffer)) {
        ErrorLog(this).Write("Failed to create vulkan stage buffer: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Get memory requirements
    vk::MemoryRequirements requirements;
    device.getBufferMemoryRequirements(stageBuffer, &requirements);

    // Allocate memory
    if (!allocator->Allocate(requirements, vk::MemoryPropertyFlagBits::eHostVisible, "stage"_id, &stageAlloc)) {
        ErrorLog(this).Write("Failed to allocate memory for stage buffer");
        return COM::kError;
    }

    // Bind to buffer
    if (vk::CheckedResult result = device.bindBufferMemory(stageBuffer, stageAlloc->deviceMemory, stageAlloc->offset)) {
        ErrorLog(this).Write("Failed to bind vulkan allocation to buffer: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Start thread
    thread.SetName("Vulkan.ResourceStager");
    thread.Start(Bindff(_Thread));

    // OK
    return COM::kOK;
}

void ResourceStager::StageBuffer(IMemoryResource *resource, void *buffer, const GHA::MemoryBounds &bounds) {
    resource->MarkAsUninitialized(0);

    PendingStage stage;
    stage.resource = resource;
    stage.buffer = buffer;
    stage.bounds = bounds;
    pendingStages.Add(stage);

    // Signal
    {
        Platform::LockGuard guard(pendingMtx);
        jobCounter++;
        pendingVar.SignalAll();
    }
}

void ResourceStager::StageTextureMip(ITexture *texture, UInt32 mip, void *buffer, const GHA::MemoryBounds &bounds) {
    texture->MarkAsUninitialized(mip);

    PendingStage stage;
    stage.resource = texture;
    stage.buffer = buffer;
    stage.bounds = bounds;
    stage.mipIndex = mip;
    pendingStages.Add(stage);

    // Signal
    {
        Platform::LockGuard guard(pendingMtx);
        jobCounter++;
        pendingVar.SignalAll();
    }
}


void ResourceStager::_Thread() {
    Platform::Event event;
    Core::Array<PendingStage> stages;
    Core::Array<PendingStage> failedLocks;

    // ...
    while(!exitFlag.Load()) {
        // Wait for a pending event
        pendingVar.Wait(pendingMtx, [&] { return jobCounter.Load() > 0 || exitFlag.Load(); });

        // Exit?
        if (exitFlag.Load()) {
            break;
        }

        // Prepare
        offset = 0;

        // Clear all stages
        stages.Clear(false);

        // Prepare command buffer
        commandBuffer.reset({});
        vk::CommandBufferBeginInfo beginInfo;
        commandBuffer.begin(&beginInfo);

        // Process all pending stages
        PendingStage job;
        while (pendingStages.Pop(job)) {
            // Attempt to schedule a lock on the resource and reschedule until available
            if ((job.resource->GetLockUserdata() != reinterpret_cast<USize>(this) && !job.resource->ScheduleLock(reinterpret_cast<USize>(this))) || job.resource->GetUsageCount() != 0) {
                // Resubmit
                failedLocks.Add(job);
                continue;
            }

            // Determine mapped range
            USize mappedWidth = job.bounds.endOffset.width - job.bounds.beginOffset.width;

            // ...
            //Allocation *allocation;
            vk::Buffer destBuffer;
            USize alignment = 16;
            switch (job.resource->GetClassID()) {
                case Texture::kCID:
                    //allocation = static_cast<Texture *>(job.resource)->GetAllocation();
                    alignment = static_cast<Texture *>(job.resource)->GetRequirements().alignment;
                    break;
                case VertexBuffer::kCID:
                    //allocation = static_cast<VertexBuffer *>(job.resource)->GetAllocation();
                    destBuffer = static_cast<VertexBuffer *>(job.resource)->GetBuffer();
                    alignment = static_cast<VertexBuffer *>(job.resource)->GetRequirements().alignment;
                    break;
                case IndexBuffer::kCID:
                    //allocation = static_cast<IndexBuffer *>(job.resource)->GetAllocation();
                    destBuffer = static_cast<IndexBuffer *>(job.resource)->GetBuffer();
                    alignment = static_cast<IndexBuffer *>(job.resource)->GetRequirements().alignment;
                    break;
                case TextureBuffer::kCID:
                    //allocation = static_cast<TextureBuffer *>(job.resource)->GetAllocation();
                    destBuffer = static_cast<TextureBuffer *>(job.resource)->GetBuffer();
                    alignment = static_cast<TextureBuffer *>(job.resource)->GetRequirements().alignment;
                    break;
                case StructuredBuffer::kCID:
                    //allocation = static_cast<StructuredBuffer *>(job.resource)->GetAllocation();
                    destBuffer = static_cast<StructuredBuffer *>(job.resource)->GetBuffer();
                    alignment = static_cast<StructuredBuffer *>(job.resource)->GetRequirements().alignment;
                    break;
                default:
                    ErrorLog(this).Write("Attempting to stage an unsupported memory resource type");
                    return;
            }

            // Align offset
            offset = Math::SnapUpper(offset, alignment);

            // Does the current staged buffer support this width?
            if ((offset + mappedWidth) >= stageAlloc->size) {
                break;
            }

            // Stage
            stages.Add(job);

            // Determine mapped offset
            // USize mappedOffset = allocation->offset + offset;

            // Map stage
            void *mapped;
            if (vk::CheckedResult result = device.mapMemory(stageAlloc->deviceMemory, offset, mappedWidth, {}, &mapped)) {
                ErrorLog(this).Write("Failed to map vulkan stage buffer: ", vk::to_string(result).c_str());
                return;
            }

            // Copy to mapped
            Core::Memory::Copy(mapped, job.buffer, mappedWidth);

            // Unmap
            device.unmapMemory(stageAlloc->deviceMemory);

            // Submit copy command
            if (job.resource->GetClassID() == Texture::kCID) {
                TextureDesc desc = static_cast<Texture *>(job.resource)->GetDesc();

                // Switch to transfer dst
                vk::ImageMemoryBarrier barrier;
                barrier.image = static_cast<Texture *>(job.resource)->GetImage();
                barrier.oldLayout = AsImageLayout(static_cast<Texture *>(job.resource)->layout[job.mipIndex]);
                barrier.newLayout = vk::ImageLayout::eTransferDstOptimal;
                barrier.subresourceRange = vk::ImageSubresourceRange(AsImageAspectFlag(desc.usage), job.mipIndex, 1, 0, 1);
                barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                AsAccessMasks(static_cast<Texture *>(job.resource)->layout[job.mipIndex], TextureLayout::eTransferDst, barrier.srcAccessMask, barrier.dstAccessMask);
                commandBuffer.pipelineBarrier(
                        vk::PipelineStageFlagBits::eAllCommands, vk::PipelineStageFlagBits::eAllCommands, {},
                        0, nullptr,
                        0, nullptr,
                        1, &barrier
                );

                // Describe region
                vk::BufferImageCopy region;
                region.imageExtent = vk::Extent3D(
                        static_cast<UInt32>(Math::Max(1, static_cast<double>(desc.size.width) / Math::Pow(2, job.mipIndex))),
                        static_cast<UInt32>(Math::Max(1, static_cast<double>(desc.size.height) / Math::Pow(2, job.mipIndex))),
                        static_cast<UInt32>(Math::Max(1, static_cast<double>(desc.size.depth) / Math::Pow(2, job.mipIndex)))
                );
                region.bufferOffset = offset;
                region.imageSubresource = vk::ImageSubresourceLayers(AsImageAspectFlag(desc.usage), job.mipIndex, 0, desc.arrayCount ? desc.arrayCount.Get() : 1);
                region.imageOffset = vk::Offset3D(0, 0, 0);

                // Copy buffer to image
                commandBuffer.copyBufferToImage(stageBuffer, static_cast<Texture *>(job.resource)->GetImage(), vk::ImageLayout::eTransferDstOptimal, 1, &region);

                // Cannot switch back to undefined
                if (static_cast<Texture *>(job.resource)->layout[job.mipIndex] == TextureLayout::eUndefined) {
                    static_cast<Texture *>(job.resource)->layout[job.mipIndex] = TextureLayout::eColorRead;
                }

                // Switch to previous
                barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
                barrier.newLayout = AsImageLayout(static_cast<Texture *>(job.resource)->layout[job.mipIndex]);
                AsAccessMasks(static_cast<Texture *>(job.resource)->layout[job.mipIndex], TextureLayout::eTransferDst, barrier.srcAccessMask, barrier.dstAccessMask);
                commandBuffer.pipelineBarrier(
                        vk::PipelineStageFlagBits::eAllCommands, vk::PipelineStageFlagBits::eAllCommands, {},
                        0, nullptr,
                        0, nullptr,
                        1, &barrier
                );
            } else {
                // Describe region
                vk::BufferCopy region;
                region.srcOffset = offset;
                region.dstOffset = job.bounds.beginOffset.width;
                region.size = mappedWidth;

                // Copy buffer to buffer
                commandBuffer.copyBuffer(stageBuffer, destBuffer, 1, &region);
            }

            // Append offset
            offset += mappedWidth;

            // Signal
            {
                Platform::LockGuard guard(pendingMtx);
                if (--jobCounter <= 0) {
                    wakeVar.SignalAll();
                }
            }
        }

        // End
        commandBuffer.end();

        // Submit this queue
        queue->workGroup.Queue([=] {
            vk::SubmitInfo submitInfo;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;
            if (vk::CheckedResult result = queue->GetQueue().submit(1, &submitInfo, fence)) {
                ErrorLog(this).Write("Failed to submit vulkan staging queue: ", vk::to_string(result).c_str());
                return;
            }
        }, &event);

        // Wait for submission
        event.Wait();

        // Wait for completion
        if (vk::CheckedResult result = device.waitForFences(1, &fence, static_cast<vk::Bool32>(true), Core::LimitsOf<UInt64>::kMax)) {
            ErrorLog(this).Write("Failed to wait for vulkan fence: ", vk::to_string(result).c_str());
            return;
        }

        // Reset
        if (vk::CheckedResult result = device.resetFences(1, &fence)) {
            ErrorLog(this).Write("Failed to reset vulkan fence: ", vk::to_string(result).c_str());
            return;
        }

        // Resubmit all staged locks
        for (auto &stage : failedLocks) {
            pendingStages.Add(stage);
        }
        failedLocks.Clear(false);

        // Process all staged jobs
        for (auto stage : stages) {
            // Mark the resource as initialized
            stage.resource->MarkAsInitialized(stage.mipIndex);
            stage.resource->ReleaseLock();
        }
    }
}

void ResourceStager::Wait() {
    wakeVar.Wait(pendingMtx, [&] { return jobCounter.Load() == 0; });
}

COM::Result ResourceStager::Released() {
    exitFlag = true;
    pendingVar.SignalAll();
    thread.Join();

    device.destroyBuffer(stageBuffer, nullptr);
    device.destroyCommandPool(pool, nullptr);
    device.destroyFence(fence, nullptr);
    allocator->Free(stageAlloc);

    // OK
    return COM::kOK;
}
