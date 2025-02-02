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
 on 12/21/2017.
//

#include <GHA/Vulkan/Interpreter.h>
#include <GHA/PipelineScheduler/Scheduler.h>
#include <Base/DefaultLogs.h>
#include <GHA/Vulkan/Queue.h>
#include <GHA/Vulkan/Device.h>
#include <GHA/Vulkan/Swapchain.h>
#include <GHA/PipelineScheduler/Command.h>
#include <GHA/Vulkan/CommandList.h>
#include <GHA/Vulkan/Texture.h>
#include <GHA/Vulkan/TextureView.h>
#include <GHA/Vulkan/VertexBuffer.h>
#include <GHA/Vulkan/IndexBuffer.h>
#include <GHA/Vulkan/PipelineLayout.h>
#include <GHA/Vulkan/Pipeline.h>
#include <GHA/Vulkan/Pass.h>
#include <GHA/Vulkan/Frame.h>
#include <GHA/Vulkan/Semaphore.h>
#include <GHA/Vulkan/DescriptorSetLayout.h>
#include <GHA/Vulkan/DescriptorSet.h>
#include <GHA/Vulkan/Shader.h>
#include <GHA/Vulkan/Sampler.h>
#include <GHA/Vulkan/ConstantBuffer.h>
#include <GHA/Vulkan/CompiledShaderBlob.h>
#include <GHA/Vulkan/Instance.h>
#include <GHA/Vulkan/Adapter.h>
#include <GHA/Vulkan/CommandPool.h>

DJINN_NS3(GHA, Vulkan);

Interpreter::Interpreter(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IInterpreter(registry, outer, host) {

}

COM::Result Interpreter::Initialize(IInstance *instance, IAdapter *adapter, IDevice *device, ISwapchain *swapchain) {
    this->instance = static_cast<Instance *>(instance);
    this->adapter = static_cast<Adapter *>(adapter);
    this->device = static_cast<Device *>(device);
    this->swapchain = static_cast<Swapchain *>(swapchain);

    // Create semaphore
    if (!this->device->CreateSemaphore(this, &renderComplete)) {
        return COM::kOK;
    }

    // Get queues
    UInt32 queueCount;
    if (!this->device->GetQueues(queueCount, nullptr)) {
        ErrorLog(this).Write("Failed to get device queues");
        return COM::kOK;
    }

    // Get queues
    queues.ResizePassive(queueCount);
    if (!this->device->GetQueues(queueCount, queues.Ptr())) {
        ErrorLog(this).Write("Failed to get device queues");
        return COM::kOK;
    }

    // Create query pool
    vk::QueryPoolCreateInfo info;
    info.queryCount = 4096;
    info.queryType = vk::QueryType::eTimestamp;
    if (vk::CheckedResult result = this->device->GetDevice().createQueryPool(&info, nullptr, &queryPool)) {
        ErrorLog(this).Write("Failed to create vulkan query pool: ", vk::to_string(result).c_str());
        return COM::kOK;
    }

    // OK
    return COM::kOK;
}


COM::Result Interpreter::Compile(IScheduler *_scheduler) {
    // As scheduler
    auto scheduler = COM::Cast<PipelineScheduler::Scheduler>(_scheduler);
    if (!scheduler) {
        ErrorLog(this).Write("Incorrect interpreter scheduler match, corrupt configuration or migration is the most likely culprit");
        return COM::kOK;
    }

    // Get resources
    UInt32 resourceCount;
    PipelineScheduler::ResourceCommand *resourceCommand;
    if (!scheduler->EnumerateResources(resourceCount, &resourceCommand)) {
        return COM::kError;
    }

    // Begin frame
    swapchain->Begin(queues[0]);

    // Process all resources
    for (USize i = 0; i < resourceCount; i++) {
        auto &command = resourceCommand[i];
        auto &payload = command.payload;

        switch (command.type) {
            case PipelineScheduler::ResourceCommandType::eNone:
                break;
            case PipelineScheduler::ResourceCommandType::eImportTexture:
                resources[payload.importTexture.uid.Get()] = payload.importTexture.texture;
                break;
            case PipelineScheduler::ResourceCommandType::eImportVertexBuffer:
                resources[payload.importVertexBuffer.uid.Get()] = payload.importVertexBuffer.buffer;
                break;
            case PipelineScheduler::ResourceCommandType::eImportIndexBuffer:
                resources[payload.importIndexBuffer.uid.Get()] = payload.importIndexBuffer.buffer;
                break;
            case PipelineScheduler::ResourceCommandType::eImportConstantBuffer:
                resources[payload.importConstantBuffer.uid.Get()] = payload.importConstantBuffer.buffer;
                break;
            case PipelineScheduler::ResourceCommandType::eCreateSampler: {
                auto &sampler = samplers[payload.createSampler.desc.Hash()];
                if (!command.isHashed || !sampler) {
                    // Lifetime managed internally
                    ISampler *_sampler;
                    if (!device->CreateSampler(nullptr, payload.createSampler.desc, &_sampler)) {
                        return COM::kError;
                    }

                    sampler = reinterpret_cast<Sampler *>(_sampler);
                }
                resources[payload.createSampler.uid.Get()] = sampler;
                break;
            }
            case PipelineScheduler::ResourceCommandType::eCreateTextureView: {
                auto texture = static_cast<ITexture *>(resources[payload.createTextureView.texture.Get()]);
                auto &view = textureViews[CombineHash(payload.createTextureView.desc.Hash(), texture->GetUID())];
                if (!command.isHashed || !view) {
                    // Lifetime managed internally
                    if (!device->CreateTextureView(nullptr, payload.createTextureView.desc, texture, &view)) {
                        return COM::kError;
                    }
                }
                resources[payload.createTextureView.uid.Get()] = view;
                break;
            }
            case PipelineScheduler::ResourceCommandType::eCreateVertexBufferView:
                break;
            case PipelineScheduler::ResourceCommandType::eCreateIndexBufferView:
                break;
            case PipelineScheduler::ResourceCommandType::eCreateStructuredBufferView:
                break;
            case PipelineScheduler::ResourceCommandType::eCreateLinearBufferView:
                break;
            case PipelineScheduler::ResourceCommandType::eCreateTextureUnorderedView:
                break;
            case PipelineScheduler::ResourceCommandType::eCreateVertexBufferUnorderedView:
                break;
            case PipelineScheduler::ResourceCommandType::eCreateIndexBufferUnorderedView:
                break;
            case PipelineScheduler::ResourceCommandType::eCreateStructuredBufferUnorderedView:
                break;
            case PipelineScheduler::ResourceCommandType::eCreateLinearBufferUnorderedView:
                break;
            case PipelineScheduler::ResourceCommandType::eCreateDescriptorSetLayout: {
                auto &layout = descriptorSetLayouts[payload.createDescriptorSetLayout.desc->Hash()];
                if (!command.isHashed || !layout) {
                    // Lifetime managed internally
                    if (!device->CreateDescriptorSetLayout(nullptr, *payload.createDescriptorSetLayout.desc, &layout)) {
                        return COM::kError;
                    }
                }
                resources[payload.createDescriptorSetLayout.uid] = layout;
                break;
            }
            case PipelineScheduler::ResourceCommandType::eCreatePipelineLayout: {
                PipelineLayoutDesc desc;
                desc.pushConstants = payload.createPipelineLayout.desc.dynamicRanges;
                for (auto &dlayout : payload.createPipelineLayout.desc.layouts) {
                    desc.AddLayout(static_cast<DescriptorSetLayout *>(resources[dlayout]));
                }

                auto &layout = pipelineLayouts[desc.Hash()];
                if (!command.isHashed || !layout) {
                    // Lifetime managed internally
                    if (!device->CreatePipelineLayout(nullptr, desc, &layout)) {
                        return COM::kError;
                    }
                }
                resources[payload.createPipelineLayout.uid] = layout;
                break;
            }
            case PipelineScheduler::ResourceCommandType::eCreatePipeline: {
                auto &pipeline = pipelines[payload.createPipeline.desc->Hash()];
                if (!command.isHashed || !pipeline) {
                    // Lifetime managed internally
                    if (!device->CreatePipeline(
                            nullptr,
                            *payload.createPipeline.desc,
                            static_cast<PipelineLayout *>(resources[payload.createPipeline.layout]),
                            static_cast<Pass *>(resources[payload.createPipeline.pass]),
                            &pipeline
                    )) {
                        return COM::kError;
                    }
                }
                resources[payload.createPipeline.uid] = pipeline;
                break;
            }
            case PipelineScheduler::ResourceCommandType::eCreatePass: {
                auto &pass = passes[payload.createPass.desc->Hash()];
                if (!command.isHashed || !pass) {
                    // Lifetime managed internally
                    if (!device->CreatePass(nullptr, *payload.createPass.desc, &pass)) {
                        return COM::kError;
                    }
                }
                resources[payload.createPass.uid.Get()] = pass;
                break;
            }
            case PipelineScheduler::ResourceCommandType::eCreateFrame: {
                FrameDesc desc;
                desc.renderArea = payload.createFrame.desc->renderArea;
                desc.layers = payload.createFrame.desc->layers;
                for (auto target : payload.createFrame.desc->renderTargets) {
                    desc.AddRenderTarget(static_cast<TextureView *>(resources[target.Get()]));
                }
                if (payload.createFrame.desc->depthStencilView.IsValid()) {
                    desc.AddRenderTarget(static_cast<TextureView *>(resources[payload.createFrame.desc->depthStencilView.Get().Get()]));
                }
                for (auto target : payload.createFrame.desc->resolveRenderTargets) {
                    desc.AddResolveRenderTarget(static_cast<TextureView *>(resources[target.Get()]));
                }
                if (payload.createFrame.desc->resolveDepthStencilView.IsValid()) {
                    desc.AddResolveRenderTarget(static_cast<TextureView *>(resources[payload.createFrame.desc->resolveDepthStencilView.Get().Get()]));
                }

                auto &frame = frames[desc.Hash()];
                if (!command.isHashed || !frame) {
                    // Lifetime managed internally
                    if (!device->CreateFrame(nullptr, desc, static_cast<Pass *>(resources[payload.createFrame.pass.Get()]), &frame)) {
                        return COM::kError;
                    }
                }
                resources[payload.createFrame.uid.Get()] = frame;
                break;
            }
            case PipelineScheduler::ResourceCommandType::eCreateDescriptorSet: {
                auto layout = static_cast<DescriptorSetLayout *>(resources[payload.createDescriptorSet.layout]);

                DescriptorSetUpdateDesc desc;
                for (auto &bind : payload.createDescriptorSet.desc->bindings) {
                    desc.AddBinding(DescriptorBinding().Binding(bind.binding).Resource(static_cast<IResource *>(resources[bind.resource])).ArrayElement(bind.arrayElement));
                    if (bind.sampler) {
                        desc.bindings.Last().Sampler(static_cast<ISampler *>(resources[bind.sampler.Get()]));
                    }
                }

                auto &set = descriptorSets[desc.Hash()];
                if (!command.isHashed || !set) {
                    // Lifetime managed internally
                    if (!device->CreateDescriptorSet(nullptr, desc, layout, &set)) {
                        return COM::kError;
                    }
                }
                resources[payload.createDescriptorSet.uid] = set;
                break;
            }
            case PipelineScheduler::ResourceCommandType::eCreateCommandList:
                break;
            case PipelineScheduler::ResourceCommandType::eCreateCommandPool:
                break;
            case PipelineScheduler::ResourceCommandType::eUpdateDescriptorSet: {
                DescriptorSetUpdateDesc desc;
                for (auto &bind : payload.updateDescriptorSet.desc->bindings) {
                    desc.AddBinding(DescriptorBinding().Binding(bind.binding).Resource(static_cast<IResource *>(resources[bind.resource])).ArrayElement(bind.arrayElement));
                    if (bind.sampler) {
                        desc.bindings.Last().Sampler(static_cast<ISampler *>(resources[bind.sampler.Get()]));
                    }
                }

                Check(static_cast<DescriptorSet *>(resources[payload.updateDescriptorSet.descriptorSet])->Update(desc));
                break;
            }
            case PipelineScheduler::ResourceCommandType::eStorePersistent:
                persistents[payload.storePersistent.persistent.Get()] = resources[payload.storePersistent.id];
                break;
            case PipelineScheduler::ResourceCommandType::eLoadPersistent:
                resources[payload.loadPersistent.id] = persistents[payload.loadPersistent.persistent.Get()];
                break;
        }
    }

    // Get queue count
    USize activeQueueCount = scheduler->GetQueueCount();
    USize backbufferCount = swapchain->GetDesc().backbufferCount.Get();
    USize activeBackbuffer = swapchain->GetCurrentIndex();

    // Ensure all queues are present
    queueData.ResizePassive(activeQueueCount);
    for (USize i = 0; i < activeQueueCount; i++) {
        queueData[i].queueIndex = static_cast<UInt32>(i);

        // ...
        queueData[i].submissions.ResizePassive(backbufferCount);
        for (USize j = 0; j < backbufferCount; j++) {
            if (!queueData[i].submissions[j].pool) {
                // Lifetime managed internally
                if (!device->CreateCommandPool(nullptr, PipelineScheduler::CommandPoolDesc(), queues[i], &queueData[i].submissions[j].pool) ||
                    !device->CreateCommandList(nullptr, PipelineScheduler::CommandListDesc(), queueData[i].submissions[j].pool, &queueData[i].submissions[j].list)) {
                    return COM::kError;
                }
            }
        }
    }

    // Foreach active queue
    for (USize queueIndex = 0; queueIndex < scheduler->GetQueueCount(); queueIndex++) {
        // Enumerate
        UInt32 commandCount;
        PipelineScheduler::Command *commands;
        if (!scheduler->EnumerateQueue(queueIndex, commandCount, &commands)) {
            ErrorLog(this).Write("Bad scheduler queue enumeration [", queueIndex, "]");
            return COM::kOK;
        }

        // Any commands?
        if (commandCount == 0) {
            continue;
        }

        // Get queue count
        auto &data = queueData[queueIndex];

        // Get active command list
        CommandList *list = data.submissions[activeBackbuffer].list;

        // Begin
        list->Reset();
        list->Begin();

        // Get stamp range
        auto stampRange = GetStampRange();

        // Performance stamp
        list->GetBuffer().resetQueryPool(queryPool, stampRange.first, 1);
        list->GetBuffer().writeTimestamp(vk::PipelineStageFlagBits::eAllCommands, queryPool, stampRange.first);

        // Translate commands
        for (USize cmdIndex = 0; cmdIndex < commandCount; cmdIndex++) {
            auto &command = commands[cmdIndex];
            switch (command.type) {
                default:
                    break;
                case PipelineScheduler::CommandTypes::eCmdBeginPass:
                    list->BeginPass(
                            static_cast<Pass *>(resources[command.cmdBeginPass.pass.Get()]),
                            static_cast<Frame *>(resources[command.cmdBeginPass.frame.Get()]),
                            command.cmdBeginPass.rect,
                            0, nullptr, nullptr
                    );
                    break;
                case PipelineScheduler::CommandTypes::eCmdEndPass:
                    list->EndPass();
                    break;
                case PipelineScheduler::CommandTypes::eCmdCopyTexture:
                    list->CopyResource(
                            static_cast<TextureView *>(resources[command.cmdCopyTexture.source.Get()]),
                            TextureLayout::eTransferSrc,
                            static_cast<TextureView *>(resources[command.cmdCopyTexture.dest.Get()]),
                            TextureLayout::eTransferDst
                    );
                    break;
                case PipelineScheduler::CommandTypes::eCmdClear:
                    break;
                case PipelineScheduler::CommandTypes::eCmdClearColor: {
                    list->ClearColor(static_cast<TextureView *>(resources[command.cmdClearColor.resource.Get()]), ColorClearValue::F32(0, 0, 0, 0));
                    break;
                }
                case PipelineScheduler::CommandTypes::eCmdClearDepthStencil: {
                    list->ClearDepthStencil(static_cast<TextureView *>(resources[command.cmdClearColor.resource.Get()]), 1.0f, 0);
                    break;
                }
                case PipelineScheduler::CommandTypes::eCmdBindPipeline:
                    list->SetPipeline(static_cast<Pipeline *>(resources[command.cmdBindPipeline.pipeline]));
                    break;
                case PipelineScheduler::CommandTypes::eCmdBindDescriptorSet: {
                    auto set = static_cast<DescriptorSet *>(resources[command.cmdBindDescriptorSet.descriptorSet]);
                    list->SetDescriptorSets(
                            command.cmdBindDescriptorSet.bindPoint,
                            static_cast<PipelineLayout *>(resources[command.cmdBindDescriptorSet.pipelineLayoutID]),
                            command.cmdBindDescriptorSet.index, 1, &set
                    );
                    break;
                }
                case PipelineScheduler::CommandTypes::eCmdBindVertexBuffers: {
                    IVertexBuffer *buffers[16];
                    for (USize i = 0; i < command.cmdBindVertexBuffer.vertexBuffers.Size(); i++) {
                        buffers[i] = static_cast<IVertexBuffer *>(resources[command.cmdBindVertexBuffer.vertexBuffers[i]]);
                    }
                    USize offsets[16] = {};
                    list->SetVertexBuffers(
                            static_cast<UInt32>(command.cmdBindVertexBuffer.vertexBuffers.Size()),
                            buffers,
                            offsets,
                            command.cmdBindVertexBuffer.offset
                    );
                    break;
                }
                case PipelineScheduler::CommandTypes::eCmdBindIndexBuffer:
                    list->SetIndexBuffer(static_cast<IIndexBuffer *>(resources[command.cmdBindIndexBuffer.indexBuffer]), 0);
                    break;
                case PipelineScheduler::CommandTypes::eCmdSetViewport:
                    list->SetViewports(&command.setViewport.viewport, 1);
                    break;
                case PipelineScheduler::CommandTypes::eCmdSetScissor:
                    list->SetScissors(&command.setScissor.scissor, 1);
                    break;
                case PipelineScheduler::CommandTypes::eCmdDraw:
                    list->Draw(
                            command.cmdDraw.vertexCount,
                            command.cmdDraw.instanceCount,
                            command.cmdDraw.vertexOffset,
                            command.cmdDraw.instanceOffset
                    );
                    break;
                case PipelineScheduler::CommandTypes::eCmdDrawIndexed:
                    list->DrawIndexed(
                            command.cmdDrawIndexed.indexCount,
                            command.cmdDrawIndexed.instanceCount,
                            command.cmdDrawIndexed.indexOffset,
                            command.cmdDrawIndexed.vertexOffset,
                            command.cmdDrawIndexed.instanceOffset
                    );
                    break;
                case PipelineScheduler::CommandTypes::eCmdDrawIndirect:
                    break;
                case PipelineScheduler::CommandTypes::eCmdDrawIndirectIndexed:
                    break;
                case PipelineScheduler::CommandTypes::eCmdDispatch:
                    break;
                case PipelineScheduler::CommandTypes::eCmdDispatchIndirect:
                    break;
                case PipelineScheduler::CommandTypes::eCmdSignalFence:
                    break;
                case PipelineScheduler::CommandTypes::eCmdWaitFence:
                    break;
                case PipelineScheduler::CommandTypes::eCmdDebugPushEvent: {
                    if (instance->GetDebugCmdBegin()) {
                        VkDebugMarkerMarkerInfoEXT info = {};
                        info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
                        info.pMarkerName = command.cmdDebugPushEvent.name;
                        instance->GetDebugCmdBegin()(list->GetBuffer(), &info);
                    }
                    break;
                }
                case PipelineScheduler::CommandTypes::eCmdDebugPopEvent: {
                    if (instance->GetDebugCmdBegin()) {
                        instance->GetDebugCmdEnd()(list->GetBuffer());
                    }
                    break;
                }
                case PipelineScheduler::CommandTypes::eCmdUpdateBuffer: {
                    auto memres = static_cast<IMemoryResource *>(resources[command.cmdUpdateConstantBuffer.buffer]);
                    if (memres->GetSize() > 0 && (command.cmdUpdateConstantBuffer.bounds.endOffset.width - command.cmdUpdateConstantBuffer.bounds.beginOffset.width) > 0) {
                        DJINN_ASSERT(
                                command.cmdUpdateConstantBuffer.bounds.endOffset.width - command.cmdUpdateConstantBuffer.bounds.beginOffset.width <= 65536, "Bad update buffer command with size ",
                                command.cmdUpdateConstantBuffer.bounds.endOffset.width - command.cmdUpdateConstantBuffer.bounds.beginOffset.width
                                );
                        list->UpdateResource(
                                memres,
                                command.cmdUpdateConstantBuffer.bounds,
                                command.cmdUpdateConstantBuffer.data
                        );
                    }
                    break;
                }
                case PipelineScheduler::CommandTypes::eCmdUpdateDynamic: {
                    list->UpdateConstant(
                            static_cast<PipelineLayout *>(resources[command.cmdUpdateDynamic.layout]),
                            command.cmdUpdateDynamic.visibleStages,
                            command.cmdUpdateDynamic.bounds,
                            command.cmdUpdateDynamic.data
                    );
                    break;
                }
                case PipelineScheduler::CommandTypes::eCmdTextureBarrier: {
                    TextureBarrierDesc barriers[16];
                    for (USize i = 0; i < command.cmdTextureBarrier.barriers->Size(); i++) {
                        barriers[i].layouts = PipelineScheduler::LayoutRange(command.cmdTextureBarrier.barriers->Get(i).from, command.cmdTextureBarrier.barriers->Get(i).to);
                        barriers[i].textureView = static_cast<TextureView *>(resources[command.cmdTextureBarrier.barriers->Get(i).texture.Get()]);
                        barriers[i].mipBounds = command.cmdTextureBarrier.barriers->Get(i).mipBounds;
                    }
                    list->SetTextureLayouts(static_cast<UInt32>(command.cmdTextureBarrier.barriers->Size()), barriers);
                    break;
                }
            }
        }

        // Performance stamp
        list->GetBuffer().resetQueryPool(queryPool, stampRange.second, 1);
        list->GetBuffer().writeTimestamp(vk::PipelineStageFlagBits::eAllCommands, queryPool, stampRange.second);

        // End
        list->End();

        // Submit
        Check(queues[data.queueIndex]->Submit(list, swapchain->GetBackbufferFence(), swapchain->GetBackbufferSemaphore(), renderComplete));
    }

    // Present
    swapchain->Present(queues[0], renderComplete);

    // Get stamps.
    if (queryStamps.Size() >= backbufferCount) {
        auto stamp = queryStamps[0];
        queryStamps.Remove(0);

        UInt64 timestamps[2];
        if (device->GetDevice().getQueryPoolResults(queryPool, stamp, 2, sizeof(timestamps), timestamps, sizeof(UInt64), vk::QueryResultFlagBits::e64) == vk::Result::eSuccess) {
            gpuDelta = (timestamps[1] - timestamps[0]) * adapter->GetProperties().limits.timestampPeriod * 1e-9;
        }
    }

    // OK
    return COM::kOK;
}

double Interpreter::GetGPUDelta() {
    return gpuDelta.Load();
}

Core::Pair<UInt32, UInt32> Interpreter::GetStampRange() {
    if ((stampOffset += 2) >= 4096) {
        stampOffset = 2;
    }
    queryStamps.Add(stampOffset - 2);
    return {stampOffset - 2, stampOffset - 1};
}

COM::Result Interpreter::Released() {
    // Wait for all queues to finish
    for (auto queue : queues) {
        if (!queue->Wait()) {
            WarningLog(this).Write("Failed to wait for queue ", queue->GetIndex(), " during shutdown, this is undefined behaviour");
        }
    }

    // Release live handles
    for (auto kv : textureViews) Check(kv.second->Release());
    for (auto kv : textures) Check(kv.second->Release());
    for (auto kv : frames) Check(kv.second->Release());
    for (auto kv : passes) Check(kv.second->Release());
    for (auto kv : samplers) Check(kv.second->Release());
    for (auto kv : descriptorSetLayouts) Check(kv.second->Release());
    for (auto kv : descriptorSets) Check(kv.second->Release());
    for (auto kv : pipelines) Check(kv.second->Release());
    for (auto kv : pipelineLayouts) Check(kv.second->Release());

    // Release queue data
    for (auto& data : queueData) {
        for (auto& sub : data.submissions) {
            Check(sub.list->Release());
            Check(sub.pool->Release());
        }
    }

    // OK
    return COM::kOK;
}
