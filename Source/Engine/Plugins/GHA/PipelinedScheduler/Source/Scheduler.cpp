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
 on 12/24/2017.
//

#include <GHA/PipelineScheduler/Scheduler.h>
#include <Base/DefaultLogs.h>
#include <GHA/ScheduledNode.h>
#include <GHA/ISwapchain.h>
#include <GHA/ITexture.h>
#include <GHA/IVertexBuffer.h>
#include <GHA/IIndexBuffer.h>
#include <GHA/IShader.h>
#include <GHA/PipelineScheduler/Command.h>
#include <GHA/ShaderBlob.h>
#include <GHA/PipelineScheduler/ICompiledShaderBlob.h>
#include <GHA/IDevice.h>
#include <GHA/IConstantBuffer.h>
#include <GHA/IStreamable.h>
#include <GHA/IResourceStager.h>
#include <GHA/PipelineScheduler/ITexture.h>

//#define COUNTS

DJINN_NS3(GHA, PipelineScheduler);

Scheduler::Scheduler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IScheduler(registry, outer, host) {

}

COM::Result Scheduler::Initialize(IDevice *device, ISwapchain *swapchain) {
    this->device = device;
    this->swapchain = swapchain;

    // Get stager
    this->stager = device->GetResourceStager();

    // Create dummy texture
    {
        GHA::ITexture *_dummyTexture;
        if (!device->CreateTexture(this, GHA::TextureDesc().Size(Size(1, 1, 1)).MipLevels(1), &_dummyTexture)) {
            return COM::kError;
        }

        // Guarenteed to be a pipeline texture
        dummyTexture = reinterpret_cast<ITexture *>(_dummyTexture);
    }

    // Create dummy vertex buffer
    {
        if (!device->CreateVertexBuffer(this, GHA::VertexBufferDesc().Count(1).Stride(4).HostVisible(true), &dummyVertexBuffer)) {
            return COM::kError;
        }

        void *ptr;
        Check(dummyVertexBuffer->Map(&ptr));
        Core::Memory::Zero(ptr, 4);
        Check(dummyVertexBuffer->Unmap(ptr));
    }

    // Create dummy index buffer
    {
        if (!device->CreateIndexBuffer(this, GHA::IndexBufferDesc().Count(1).Type(IndexType::eU32).HostVisible(true), &dummyIndexBuffer)) {
            return COM::kError;
        }

        void *ptr;
        Check(dummyIndexBuffer->Map(&ptr));
        Core::Memory::Zero(ptr, static_cast<USize>(IndexType::eU32));
        Check(dummyIndexBuffer->Unmap(ptr));
    }

    // Prepare queues
    queues.Resize(1);

    // Prepare frames
    frames.Resize(swapchain->GetDesc().backbufferCount.Get());

    // OK
    return COM::kOK;
}


COM::Result Scheduler::Schedule(UInt32 count, ScheduledNode **nodes) {
    for (UInt32 i = 0; i < count; i++) {
        scheduledNodes.Add(nodes[i]);
    }

    // OK
    return COM::kOK;
}

COM::Result Scheduler::Compile() {
#ifdef COUNTS
    static int i = 0;
    if (i++ >= 50) {
        InfoLog(this).Write("========================");
        i = 0;

        for (auto kv : counts) {
            InfoLog(this).Write(kv.first, " : ", kv.second);
        }
    }
    counts.Clear();
#endif

    FrameData *frame = &frames[swapchain->GetCurrentIndex()];

    // Perform garbage collection
    if (!GarbageCollect(frame)) {
        return COM::kError;
    }

    // Flush completed frame
    if (!Flush(frame)) {
        return COM::kError;
    }

    // MRL Analysis
    if (!AnalyzeMutableResourceLifetime(frame)) {
        return COM::kError;
    }

    // Resource allocation
    if (!AllocateResources(&queues[0], frame)) {
        return COM::kError;
    }

    // Generate commands
    if (!GenerateCommands(frame)) {
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result Scheduler::CompileNode(FrameData *frame, Scheduler::QueueBucket *bucket, ScheduledNode *node) {
    // Flush states
    bindState.Flush();
    pipelineState.Flush();
    frameState.Flush();
    passState.Flush();

    // Clear bindings
    boundPipeline = {};
    boundDescriptorSets = {};
    boundIndexBuffer = {};
    boundVertexBuffers = {};
    lastPass = {};
    currentShaderLayout = {};

    // Default pipeline
    pipelineState.desc.AddDynamicStates(DynamicState::eViewport);
    pipelineState.desc.AddDynamicStates(DynamicState::eScissor);

    bucket->commands.Add(Command::CmdDebugPushEvent(node->name));

    for (auto &command : node->commands) {
        switch (command.type) {
            case CommandType::eDebugPushEvent:
                break;
            case CommandType::eDebugPopEvent:
                break;
            case CommandType::eCmdCopyTexture: {
                TextureViewID va = CreateTextureView(command.cmdCopyTexture.source, {});
                Barrier(bucket, va, TextureLayout::eTransferSrc);
                TextureViewID vb = CreateTextureView(command.cmdCopyTexture.dest, {});
                Barrier(bucket, vb, TextureLayout::eTransferDst);
                bucket->commands.Add(Command::CmdCopyTexture(va, vb));
                break;
            }
            case CommandType::eCmdClear:
                break;
            case CommandType::eCmdClearColor: {
                TextureViewID uid = CreateTextureView(command.cmdClearColor.resource, {});
                Barrier(bucket, uid, TextureLayout::eTransferDst);
                bucket->commands.Add(Command::CmdClearColor(uid));
                if (auto ms = textureMs[textureDescs[command.cmdClearColor.resource.Get()].texture]) {
                    uid = CreateTextureView(ImportTexture(ms), {});
                    Barrier(bucket, uid, TextureLayout::eTransferDst);
                    bucket->commands.Add(Command::CmdClearColor(uid));
                }
                break;
            }
            case CommandType::eCmdClearDepthStencil: {
                TextureViewID uid = CreateTextureView(command.cmdClearDepthStencil.resource, {});
                Barrier(bucket, uid, TextureLayout::eTransferDst);
                bucket->commands.Add(Command::CmdClearDepthStencil(uid));
                if (auto ms = textureMs[textureDescs[command.cmdClearDepthStencil.resource.Get()].texture]) {
                    uid = CreateTextureView(ImportTexture(ms), {});
                    Barrier(bucket, uid, TextureLayout::eTransferDst);
                    bucket->commands.Add(Command::CmdClearDepthStencil(uid));
                }
                break;
            }
            case CommandType::eBindRenderTarget: {
                passState.desc.colorRenderTargets.ResizePassive(command.bindRenderTarget.offset + 1);
                frameState.desc.renderTargets.ResizePassive(command.bindRenderTarget.offset + 1);
                if (pipelineState.desc.blend.renderTargets.ResizePassive(passState.desc.colorRenderTargets.Size())) {
                    pipelineState.Dirty();
                }

                if (pipelineState.desc.blend.renderTargets[command.bindRenderTarget.offset].Hash() != command.bindRenderTarget.blendDesc.Hash()) {
                    pipelineState.desc.blend.renderTargets[command.bindRenderTarget.offset] = command.bindRenderTarget.blendDesc;
                    pipelineState.Dirty();
                }

                TextureViewID viewID = CreateTextureView(command.bindRenderTarget.texture, command.bindRenderTarget.viewDesc);

                passState.desc.colorRenderTargets[command.bindRenderTarget.offset] = (
                        RenderTargetDesc()
                                .InitialLayout(TextureLayout::eColorRenderTarget)
                                .FinalLayout(TextureLayout::eColorRenderTarget)
                                .Format(command.bindRenderTarget.viewDesc.format == Format::eUndefined ? textureDescs[command.bindRenderTarget.texture.Get()].desc.format
                                                                                                       : command.bindRenderTarget.viewDesc.format)
                );

                frameState.desc.renderArea.extent = textureDescs[command.bindRenderTarget.texture.Get()].desc.size;
                frameState.desc.renderTargets[command.bindRenderTarget.offset] = viewID;

                passState.Dirty();
                frameState.Dirty();
                bindState.Dirty();
                break;
            }
            case CommandType::eBindDepthRenderTarget: {
                passState.desc.depthStencilTarget = RenderTargetDesc()
                        .InitialLayout(TextureLayout::eDepthStencilRenderTarget)
                        .FinalLayout(TextureLayout::eDepthStencilRenderTarget)
                        .Format(command.bindDepthRenderTarget.viewDesc.format == Format::eUndefined ? textureDescs[command.bindDepthRenderTarget.texture.Get()].desc.format
                                                                                                    : command.bindDepthRenderTarget.viewDesc.format);
                passState.Dirty();

                TextureViewID viewID = CreateTextureView(command.bindDepthRenderTarget.texture, command.bindDepthRenderTarget.viewDesc);

                frameState.desc.depthStencilView = viewID;
                frameState.Dirty();
                bindState.Dirty();
                break;
            }
            case CommandType::eBindResource:
                break;
            case CommandType::eBindTexture: {
                FlushLayout(bucket);

                auto& desc = textureDescs[command.bindTexture.texture.Get()];
                desc.isBound = true;

                TextureViewID viewID = CreateTextureView(command.bindTexture.texture, command.bindTexture.viewDesc);

                SamplerID samplerID = {context.CreateUID()};
                resources.Add(ResourceCommand::CreateSampler(command.bindTexture.samplerDesc, samplerID));

                if (command.bindTexture.arrayIndex != -1) {
                    auto &binds = bindState.arrayBinds[command.bindTexture.bindID];
                    if (!binds.Any([&](DescriptorBinding &bind) {
                        if (static_cast<Int32>(bind.arrayElement) == command.bindTexture.arrayIndex) {
                            bind = DescriptorBinding().Resource(viewID).Sampler(samplerID).ArrayElement(static_cast<UInt32>(command.bindTexture.arrayIndex));
                            return true;
                        }
                        return false;
                    })) {
                        binds.Add(DescriptorBinding().Resource(viewID).Sampler(samplerID).ArrayElement(static_cast<UInt32>(command.bindTexture.arrayIndex)));
                    }
                } else {
                    bindState.binds[command.bindTexture.bindID] = DescriptorBinding().Resource(viewID).Sampler(samplerID).ArrayElement(0);
                }
                bindState.Dirty();
                break;
            }
            case CommandType::eBindPacked: {
                FlushLayout(bucket);
                EndRenderWork(bucket);

                auto &buffer = constantBuffers[CombineHash(command.bindPacked.size, command.bindPacked.bindID.Get())];
                if (!buffer) {
                    // Lifetime managed internally
                    if (!device->CreateConstantBuffer(nullptr, ConstantBufferDesc().AllocationSize(command.bindPacked.size), &buffer)) {
                        return COM::kError;
                    }
                }

                ConstantBufferID uid = {context.CreateUID()};
                resources.Add(ResourceCommand::ImportConstantBuffer(buffer, uid));
                bucket->commands.Add(Command::CmdUpdateBuffer(uid, &context.constantBuffer[command.bindPacked.constantOffset], MemoryBounds(command.bindPacked.size)));

                bindState.binds[command.bindPacked.bindID] = DescriptorBinding().Resource(uid).ArrayElement(0);
                bindState.Dirty();
                break;
            }
            case CommandType::eBindDynamic: {
                FlushLayout(bucket);
                dynamicState.Dirty();
                dynamicState.map[command.bindDynamic.bindID] = {&context.constantBuffer[command.bindDynamic.constantOffset], command.bindDynamic.size};
                break;
            }
            case CommandType::eBindVertexBuffer: {
                boundVertexBuffers.ResizePassive(command.bindVertexBuffer.offset + 1);
                if (boundVertexBuffers[command.bindVertexBuffer.offset] != command.bindVertexBuffer.vertexBuffer) {
                    boundVertexBuffers[command.bindVertexBuffer.offset] = command.bindVertexBuffer.vertexBuffer;

                    auto cmd = Command::CmdBindVertexBuffer(command.bindVertexBuffer.offset);
                    cmd.vertexBuffers.Add(command.bindVertexBuffer.vertexBuffer);
                    bucket->commands.Add(cmd);
                }
                break;
            }
            case CommandType::eBindIndexBuffer: {
                if (boundIndexBuffer != command.bindIndexBuffer.buffer) {
                    boundIndexBuffer = command.bindIndexBuffer.buffer;
                    bucket->commands.Add(Command::CmdBindIndexBuffer(command.bindIndexBuffer.buffer));
                }
                break;
            }
            case CommandType::eSetRasterizerState:
                pipelineState.desc.rasterizer = command.setRasterizerState.desc;
                pipelineState.Dirty();
                break;
            case CommandType::eSetSampleMode:
                pipelineState.desc.multiSample.sampleMode = command.setSampleMode.sampleMode;
                pipelineState.Dirty();
                frameState.Dirty();
                passState.Dirty();
                break;
            case CommandType::eSetDepthStencilState:
                pipelineState.desc.depthStencil = command.setDepthStencilState.desc;
                pipelineState.Dirty();
                break;
            case CommandType::eSetTopology:
                pipelineState.desc.topology = command.setTopology.topology;
                pipelineState.Dirty();
                break;
            case CommandType::eSetViewport: {
                bucket->commands.Add(Command::SetViewport(command.setViewport.offset, command.setViewport.viewport));
                break;
            }
            case CommandType::eSetScissor: {
                bucket->commands.Add(Command::SetScissor(command.setScissor.offset, command.setScissor.scissor));
                break;
            }
            case CommandType::eBindShader: {
                if (!pipelineState.desc.shaders.Contains(command.bindShader.shader)) {
#ifdef COUNTS
                    counts["BindShader"]++;
#endif
                    bool ignore = false;
                    for (auto &shader : pipelineState.desc.shaders) {
                        if (shader->GetDesc().blob->stage == command.bindShader.shader->GetDesc().blob->stage) {
                            shader = command.bindShader.shader;
                            pipelineState.Dirty();
                            currentShaderLayout = nullptr;
                            ignore = true;
                        }
                    }

                    if (!ignore) {
                        pipelineState.desc.shaders.Add(command.bindShader.shader);
                        pipelineState.Dirty();
                        currentShaderLayout = nullptr;
                    }
                    if (command.bindShader.shader->GetDesc().blob->stage == GHA::Stage::eVertex) {
                        pipelineState.desc.inputAssembly = command.bindShader.shader->GetDesc().blob->inputAssembly;
                        pipelineState.Dirty();
                        currentShaderLayout = nullptr;
                    }
                }
                break;
            }
            case CommandType::eBindShaderBlob: {
                auto &shader = shaders[command.bindShaderBlob.blob];
                if (!shader) {
                    // Lifetime managed internally
                    if (!device->CreateShader(nullptr, ShaderDesc().Blob(command.bindShaderBlob.blob), &shader)) {
                        return COM::kError;
                    }
                }

                if (!pipelineState.desc.shaders.Contains(shader)) {
#ifdef COUNTS
                    counts["BindShader"]++;
#endif
                    bool ignore = false;
                    for (auto &_shader : pipelineState.desc.shaders) {
                        if (_shader->GetDesc().blob->stage == shader->GetDesc().blob->stage) {
                            _shader = shader;
                            pipelineState.Dirty();
                            currentShaderLayout = nullptr;
                            ignore = true;
                        }
                    }

                    if (!ignore) {
                        pipelineState.desc.shaders.Add(shader);
                        pipelineState.Dirty();
                        currentShaderLayout = nullptr;
                    }
                    if (shader->GetDesc().blob->stage == GHA::Stage::eVertex) {
                        pipelineState.desc.inputAssembly = shader->GetDesc().blob->inputAssembly;
                        pipelineState.Dirty();
                    }
                }
                break;
            }
            case CommandType::eDraw: {
                FlushState(frame, bucket);
                bucket->commands.Add(Command::CmdDraw(command.draw.vertexCount, command.draw.instanceCount, command.draw.vertexOffset, command.draw.instanceOffset));
                break;
            }
            case CommandType::eDrawIndexed: {
                FlushState(frame, bucket);
                bucket->commands.Add(Command::CmdDrawIndexed(
                        command.drawIndexed.indexCount,
                        command.drawIndexed.instanceCount,
                        command.drawIndexed.vertexOffset,
                        command.drawIndexed.indexOffset,
                        command.drawIndexed.instanceOffset
                ));
                break;
            }
            case CommandType::eDrawIndirect:
                break;
            case CommandType::eDrawIndirectIndexed:
                break;
            case CommandType::eDispatch:
                break;
            case CommandType::eDispatchIndirect:
                break;
            case CommandType::eSignalFence:
                break;
            case CommandType::eWaitFence:
                break;
            default:
                ErrorLog(this).Write("Incorrect scheduled gha command");
                return COM::kError;
        }
    }

    EndRenderWork(bucket);

    bucket->commands.Add(Command::CmdDebugPopEvent());

    // ...
    return COM::kOK;
}

void Scheduler::FlushLayout(QueueBucket *bucket) {
    // TODO: ShaderUIDs
    // Generate shader combination hash
    USize hash = 0;
    for (auto &shader : pipelineState.desc.shaders) {
        hash = CombineHash(hash, shader);
    }

    // Not compiled?
    if (!compiledShaderLayouts.Contains(hash)) {
        auto &compiledLayout = compiledShaderLayouts[hash];

#ifdef COUNTS
        counts["PipelineBindState"]++;
#endif

        compiledLayout.layoutDescs.Clear();
        for (auto &shader : pipelineState.desc.shaders) {
            for (const auto &kv : reinterpret_cast<PipelineScheduler::ICompiledShaderBlob *>(shader->GetBlob())->resources) {
                if (compiledLayout.layoutDescs.Size() <= kv.first) {
                    compiledLayout.layoutDescs.Resize(kv.first + 1);
                }

                for (auto &binding : kv.second) {
                    USize index;
                    if (compiledLayout.layoutDescs[kv.first].bindings.Any([&](const DescriptorLayoutBinding &bind) { return bind.binding == binding.index; }, index)) {
                        compiledLayout.layoutDescs[kv.first].bindings[index].AddStage(shader->GetDesc().blob->stage).Count(1).Class(binding.rid);
                    } else {
                        compiledLayout.layoutDescs[kv.first].AddBinding(
                                DescriptorLayoutBinding()
                                        .Binding(binding.index)
                                        .Stages(shader->GetDesc().blob->stage)
                                        .Count(binding.count ?: 1)
                                        .Class(binding.rid)
                        );
                    }

                    compiledLayout.pipelineBindState.map[BindID(Hash(binding.name))] = PipelineBinding(binding.set, binding.index);
                    compiledLayout.pipelineBindState.mapR[CombineHash(binding.set, binding.index)] = BindID(Hash(binding.name));
                }
            }
        }

        //bindState.binds.RemoveAll([&](auto &kv) { return !pipelineBindState.map.Contains(kv.first); });

        for (auto &shader : pipelineState.desc.shaders) {
            auto blob = shader->GetDesc().blob;
            auto compiledBlob = reinterpret_cast<PipelineScheduler::ICompiledShaderBlob *>(shader->GetBlob());

            if (compiledBlob->dynamicBindingOffsets.Size() > 0) {
                DynamicRange range;
                range.stage = blob->stage;
                range.size = compiledBlob->dynamicSize;
                range.offset = compiledBlob->dynamicOffset;
                compiledLayout.pipelineLayoutState.desc.dynamicRanges.Add(range);

                for (const auto &binding : compiledBlob->dynamicBindingOffsets) {
                    compiledLayout.pipelineDynamicState.stageVisibility[BindID{binding.first}] += blob->stage;
                    compiledLayout.pipelineDynamicState.offsets[BindID{binding.first}] = binding.second;
                }
            }
        }
        //descriptorSetTemplates

        compiledLayout.descriptorLayoutTemplate = templatePool.PopConstruct();

        for (USize i = 0; i < compiledLayout.layoutDescs.Size(); i++) {
            DescriptorSetUpdateDesc update;
            for (auto &bind : compiledLayout.layoutDescs[i].bindings) {
                compiledLayout.descriptorLayoutTemplate->offsets[compiledLayout.pipelineBindState.mapR[CombineHash(i, bind.binding)]] = static_cast<UInt32>(update.bindings.Size());

                for (UInt32 element = 0; element < bind.count; element++) {
                    switch (bind.id) {
                        default:
                            WarningLog(this).Write("Unexpected descriptor type of none, possibly buggy descriptor set template");
                            break;
                        case DescriptorType::eConstantBuffer:
                            update.AddBinding(
                                    DescriptorBinding()
                                            .Class(DescriptorType::eConstantBuffer)
                                            .Binding(PipelineBinding(static_cast<UInt32>(i), bind.binding))
                                            .ArrayElement(element)
                            );
                            break;
                        case DescriptorType::eTextureView: {
                            update.AddBinding(
                                    DescriptorBinding()
                                            .Class(DescriptorType::eTextureView)
                                            .Binding(PipelineBinding(static_cast<UInt32>(i), bind.binding))
                                            .ArrayElement(element)
                            );
                        }
                            break;
                        case DescriptorType::eUnorderedTextureView:
                            update.AddBinding(
                                    DescriptorBinding()
                                            .Class(DescriptorType::eUnorderedTextureView)
                                            .Binding(PipelineBinding(static_cast<UInt32>(i), bind.binding))
                                            .ArrayElement(element)
                            );
                            break;
                        case DescriptorType::eUnorderedVertexBufferView:
                            update.AddBinding(
                                    DescriptorBinding()
                                            .Class(DescriptorType::eUnorderedVertexBufferView)
                                            .Binding(PipelineBinding(static_cast<UInt32>(i), bind.binding))
                                            .ArrayElement(element)
                            );
                            break;
                        case DescriptorType::eUnorderedIndexBufferView:
                            update.AddBinding(
                                    DescriptorBinding()
                                            .Class(DescriptorType::eUnorderedIndexBufferView)
                                            .Binding(PipelineBinding(static_cast<UInt32>(i), bind.binding))
                                            .ArrayElement(element)
                            );
                            break;
                        case DescriptorType::eStructuredBufferView:
                            update.AddBinding(
                                    DescriptorBinding()
                                            .Class(DescriptorType::eStructuredBufferView)
                                            .Binding(PipelineBinding(static_cast<UInt32>(i), bind.binding))
                                            .ArrayElement(element)
                            );
                            break;
                        case DescriptorType::eUnorderedStructuredBufferView:
                            update.AddBinding(
                                    DescriptorBinding()
                                            .Class(DescriptorType::eUnorderedStructuredBufferView)
                                            .Binding(PipelineBinding(static_cast<UInt32>(i), bind.binding))
                                            .ArrayElement(element)
                            );
                            break;
                        case DescriptorType::eTextureBufferView:
                            update.AddBinding(
                                    DescriptorBinding()
                                            .Class(DescriptorType::eTextureBufferView)
                                            .Binding(PipelineBinding(static_cast<UInt32>(i), bind.binding))
                                            .ArrayElement(element)
                            );
                            break;
                        case DescriptorType::eUnorderedTextureBufferView:
                            update.AddBinding(
                                    DescriptorBinding()
                                            .Class(DescriptorType::eUnorderedTextureBufferView)
                                            .Binding(PipelineBinding(static_cast<UInt32>(i), bind.binding))
                                            .ArrayElement(element)
                            );
                            break;
                    }
                }
            }

            compiledLayout.descriptorLayoutTemplate->descs.Add(update);
        }
    }

    if (!currentShaderLayout) {
#ifdef COUNTS
        counts["CurrentShaderLayout"]++;
#endif

        // Set current layout
        currentShaderLayout = &compiledShaderLayouts[hash];

        // Populate descriptor set layouts
        currentShaderLayout->pipelineLayoutState.desc.layouts.Clear();
        for (auto &desc : currentShaderLayout->layoutDescs) {
            auto ptr = descriptorSetLayoutPools.PopConstruct();
            *ptr = desc;

            ResourceID id(context.CreateUID());
            resources.Add(ResourceCommand::CreateDescriptorSetLayout(ptr, id));
            currentShaderLayout->pipelineLayoutState.desc.AddLayout(id);
        }

        // Fill dummy bindings
        for (USize i = 0; i < currentShaderLayout->descriptorLayoutTemplate->descs.Size(); i++) {
            for (auto &bind : currentShaderLayout->descriptorLayoutTemplate->descs[i].bindings) {
                switch (bind.id) {
                    case DescriptorType::eTextureView: {
                        SamplerID samplerID = {context.CreateUID()};
                        resources.Add(ResourceCommand::CreateSampler(GHA::Samplers::noMip, samplerID));

                        auto view = CreateTextureView(ImportTexture(dummyTexture), GHA::TextureViews::none);
                        Barrier(bucket, view, TextureLayout::eColorRead);

                        bind.resource = view;
                        bind.sampler = samplerID;
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        // Descriptor uid
        auto uid = ResourceID(context.CreateUID());

        // Create pipeline layout
        resources.Add(ResourceCommand::CreatePipelineLayout(currentShaderLayout->pipelineLayoutState.desc, currentShaderLayout->pipelineLayoutState.id = uid));
    }
}

void Scheduler::FlushState(FrameData *frame, Scheduler::QueueBucket *bucket) {
    FlushLayout(bucket);

    if (bindState.barrierDirty) {
        for (const auto &bind : bindState.binds) {
            auto tvid = TextureViewID(bind.second.resource);
            if (textureViewDescs.Contains(tvid.Get())) {
                Barrier(bucket, tvid, TextureLayout::eColorRead);
            }
        }

        for (const auto &bind : bindState.arrayBinds) {
            for (const auto &element : bind.second) {
                auto tvid = TextureViewID(element.resource);
                if (textureViewDescs.Contains(tvid.Get())) {
                    Barrier(bucket, tvid, TextureLayout::eColorRead);
                }
            }
        }

        for (auto target : frameState.desc.renderTargets) {
            Barrier(bucket, target, TextureLayout::eColorRenderTarget);
        }

        if (frameState.desc.depthStencilView.IsValid()) {
            Barrier(bucket, frameState.desc.depthStencilView.Get(), TextureLayout::eDepthStencilRenderTarget);
        }

        bindState.BarrierClean();
    }

    if (bindState.dirty) {
#ifdef COUNTS
        counts["BindState"]++;
#endif
        auto _template = currentShaderLayout->descriptorLayoutTemplate;

        Core::FixedArray12<DescriptorSetUpdateDesc *> descriptorSetDescs;
        descriptorSetDescs.Resize(currentShaderLayout->pipelineLayoutState.desc.layouts.Size());
        for (USize i = 0; i < descriptorSetDescs.Size(); i++) {
            descriptorSetDescs[i] = descriptorSetUpdatePools.PopConstruct();
            *descriptorSetDescs[i] = _template->descs[i];
        }

        bool update = false;

        for (const auto &bind : bindState.binds) {
            auto it = currentShaderLayout->pipelineBindState.map.Find(bind.first);
            if (it == currentShaderLayout->pipelineBindState.map.End()) {
                continue;
            }

            bind.second.Binding((*it).second);
            descriptorSetDescs[bind.second.binding.set]->bindings[_template->offsets[bind.first]] = bind.second;
        }
        for (const auto &binds : bindState.arrayBinds) {
            auto it = currentShaderLayout->pipelineBindState.map.Find(binds.first);
            if (it == currentShaderLayout->pipelineBindState.map.End()) {
                continue;
            }

            for (auto &element : binds.second) {
                update = true;
                element.Binding((*it).second);
                descriptorSetDescs[element.binding.set]->bindings[_template->offsets[binds.first] + element.arrayElement] = element;
            }
        }

        bindState.ids.Resize(descriptorSetDescs.Size());
        for (USize i = 0; i < descriptorSetDescs.Size(); i++) {
            if (update) {
                USize bucketHash = currentShaderLayout->layoutDescs[i].Hash();
                auto &descBucket = descriptorBuckets[bucketHash];
                if (!descBucket) {
                    descBucket = descriptorBucketPool.PopConstruct();
                }

                FrameData::DescriptorSet set;
                if (descBucket->retiredSets.Size() == 0) {
                    resources.Add(ResourceCommand::CreateDescriptorSet(descriptorSetDescs[i], currentShaderLayout->pipelineLayoutState.desc.layouts[i],
                                                                       bindState.ids[i] = ResourceID(context.CreateUID())).Hashed(false));
                    set.id = CreatePersistent(bindState.ids[i]);
                } else {
                    ResourceID id = LoadPersistent(set.id = descBucket->retiredSets.PopEnd());
                    resources.Add(ResourceCommand::UpdateDescriptorSet(id, descriptorSetDescs[i]));
                    bindState.ids[i] = id;
                }

                set.bucket = bucketHash;
                frame->descriptorSets.Add(set);
            } else {
                resources.Add(
                        ResourceCommand::CreateDescriptorSet(descriptorSetDescs[i], currentShaderLayout->pipelineLayoutState.desc.layouts[i], bindState.ids[i] = ResourceID(context.CreateUID())));
            }
        }

        bindState.Clean();
    }

    if (passState.dirty) {
#ifdef COUNTS
        counts["PassState"]++;
#endif
        auto desc = passPools.PopConstruct();
        *desc = passState.desc;
        if (pipelineState.desc.multiSample.sampleMode != GHA::SampleMode::e1) {
            desc->colorResolveRenderTargets = desc->colorRenderTargets;
            for (auto &target : desc->colorRenderTargets) {
                target.SampleMode(pipelineState.desc.multiSample.sampleMode);
            }
            desc->depthResolveStencilTarget = desc->depthStencilTarget;
            desc->depthStencilTarget->SampleMode(pipelineState.desc.multiSample.sampleMode);
        }
        resources.Add(ResourceCommand::CreatePass(desc, passState.id = {context.CreateUID()}));
        passState.Clean();
    }

    if (pipelineState.dirty) {
#ifdef COUNTS
        counts["PipelineState"]++;
#endif
        auto desc = pipelinePools.PopConstruct();
        *desc = pipelineState.desc;
        resources.Add(ResourceCommand::CreatePipeline(desc, currentShaderLayout->pipelineLayoutState.id, passState.id, pipelineState.id = ResourceID(context.CreateUID())));
        pipelineState.Clean();
    }

    if (dynamicState.dirty) {
#ifdef COUNTS
        counts["PipelineDynamicState"]++;
#endif
        for (const auto &kv : dynamicState.map) {
            bucket->commands.Add(Command::CmdUpdateDynamic(
                    currentShaderLayout->pipelineLayoutState.id,
                    currentShaderLayout->pipelineDynamicState.stageVisibility[kv.first],
                    kv.second.first,
                    {currentShaderLayout->pipelineDynamicState.offsets[kv.first], kv.second.second}
            ));
        }

        dynamicState.Clean();
    }

    if (frameState.dirty) {
#ifdef COUNTS
        counts["FrameState"]++;
#endif
        auto desc = framePools.PopConstruct();
        *desc = frameState.desc;
        if (pipelineState.desc.multiSample.sampleMode != GHA::SampleMode::e1) {
            desc->resolveRenderTargets = desc->renderTargets;
            for (auto &target : desc->renderTargets) {
                IntermediateTextureViewDesc intermView = textureViewDescs[target.Get()];
                IntermediateTextureDesc &interm = textureDescs[intermView.textureID.Get()];
                auto &msTexture = textureMs[interm.texture];
                if (!msTexture) {
                    // Lifetime managed internally
                    GHA::ITexture *texture;
                    Sink(device->CreateTexture(nullptr, GHA::TextureDesc(interm.desc).Samples(pipelineState.desc.multiSample.sampleMode), &texture));

                    msTexture = static_cast<PipelineScheduler::ITexture *>(texture);
                }
                target = CreateTextureView(ImportTexture(msTexture), GHA::TextureViewDesc(intermView.desc));
                if (!msTexture->IsInitialized()) {
                    Barrier(bucket, target, TextureLayout::eTransferDst);
                    bucket->commands.Add(Command::CmdClearColor(target));
                    msTexture->MarkAllAsInitialized();
                }
                Barrier(bucket, target, TextureLayout::eColorRenderTarget);
            }

            desc->resolveDepthStencilView = desc->depthStencilView;
            if (desc->depthStencilView.IsValid()) {
                IntermediateTextureViewDesc intermView = textureViewDescs[desc->depthStencilView.Get().Get()];
                IntermediateTextureDesc &interm = textureDescs[intermView.textureID.Get()];
                auto &msTexture = textureMs[interm.texture];
                if (!msTexture) {
                    // Lifetime managed internally
                    GHA::ITexture *texture;
                    Sink(device->CreateTexture(nullptr, GHA::TextureDesc(interm.desc).Samples(pipelineState.desc.multiSample.sampleMode), &texture));

                    msTexture = static_cast<PipelineScheduler::ITexture *>(texture);
                }
                desc->depthStencilView = CreateTextureView(ImportTexture(msTexture), GHA::TextureViewDesc(intermView.desc));
                if (!msTexture->IsInitialized()) {
                    Barrier(bucket, desc->depthStencilView.Get(), TextureLayout::eTransferDst);
                    bucket->commands.Add(Command::CmdClearDepthStencil(desc->depthStencilView.Get()));
                    msTexture->MarkAllAsInitialized();
                }
                Barrier(bucket, desc->depthStencilView.Get(), TextureLayout::eDepthStencilRenderTarget);
            }
        }
        resources.Add(ResourceCommand::CreateFrame(desc, passState.id, frameState.id = {context.CreateUID()}));
        frameState.Clean();
    }

    if (lastPass != passState.id) {
        EndRenderWork(bucket);

        lastPass = passState.id;

        Rect passRect;
        if (frameState.desc.renderTargets.Size() > 0) {
            passRect = textureViewDescs[frameState.desc.renderTargets[0].Get()].texture->GetDesc().size;
        }
        bucket->commands.Add(Command::CmdBeginPass(frameState.id, passState.id, passRect));
    }

    if (boundPipeline != pipelineState.id) {
        boundPipeline = pipelineState.id;
        bucket->commands.Add(Command::CmdBindPipeline(pipelineState.id));
    }

    boundDescriptorSets.ResizePassive(currentShaderLayout->pipelineLayoutState.desc.layouts.Size());
    for (USize i = 0; i < boundDescriptorSets.Size(); i++) {
        if (boundDescriptorSets[i] != bindState.ids[i]) {
            bucket->commands.Add(Command::CmdBindDescriptorSet(
                    boundDescriptorSets[i] = bindState.ids[i],
                    currentShaderLayout->pipelineLayoutState.id,
                    GHA::BindPoint::eGraphics,
                    static_cast<UInt32>(i)
            ));
        }
    }
}

void Scheduler::EndRenderWork(Scheduler::QueueBucket *bucket) {
    if (lastPass != nullptr) {
        Command end;
        end.cmdEndPass = {};
        bucket->commands.Add(end);
        lastPass = nullptr;
    }
}

SchedulerContext *Scheduler::GetContext() {
    return &context;
}

void Scheduler::Barrier(QueueBucket *queue, const TextureViewID &uid, TextureLayout to) {
    DJINN_ASSERT(textureViewDescs.Contains(uid.Get()));

    auto &desc = textureViewDescs[uid.Get()];

    auto& baseDesc = textureDescs[desc.textureID.Get()];

    MipBounds bounds = desc.desc.mipBounds.IsValid() ? desc.desc.mipBounds.Get() : MipBounds(0, desc.texture->GetDesc().mipLevels);

    // Create barriers
    Command::CmdTextureBarrier cmd = {};
    cmd.barriers = barrierPools.PopConstruct();
    for (UInt32 mip = bounds.lower; mip < bounds.upper; mip++) {
        auto &layout = desc.texture->layout[mip];
        if (layout != to) {
            if (mip == bounds.lower || cmd.barriers->Last().from != layout || cmd.barriers->Last().mipBounds.upper != mip) {
                cmd.barriers->Add(Command::CmdTextureBarrier::Barrier(
                        uid,
                        {mip, mip + 1},
                        layout,
                        to
                ));
            } else {
                cmd.barriers->Last().mipBounds.upper++;
            }

            layout = to;
        }
    }

#ifdef COUNTS
    counts["Barrier"]++;
#endif

    if (cmd.barriers->Size() > 0) {
#ifdef COUNTS
        counts["IssuedBarrier"]++;
#endif
        queue->commands.Add(cmd);

        if (baseDesc.isBound) {
            bindState.BarrierDirty();
        }
    }
}

COM::Result Scheduler::Flush(FrameData *frame) {
    // Remove the frames usages
    for (auto resource : frame->memoryResources) {
        resource->RemoveUsage();
    }
    frame->memoryResources.Clear();

    // Remove the frames deferred resources
    for (auto resource : frame->deferredDeletes) {
        if (COM::CheckedResult result = resource->Release()) {
            return result;
        }
    }
    frame->deferredDeletes.Clear();

    // Free the remaining descriptors
    for (auto descriptor : frame->descriptorSets) {
        descriptorBuckets[descriptor.bucket]->retiredSets.Add(descriptor.id);
    }
    frame->descriptorSets.Clear();

    // Prepare queues
    for (auto &queue : queues) {
        queue.commands.Clear();
    }

    // Clear
    failedNodes.Clear();

    // Clear pools
    barrierPools.Flush();
    descriptorSetLayoutPools.Flush();
    descriptorSetUpdatePools.Flush();
    passPools.Flush();
    framePools.Flush();
    pipelinePools.Flush();
    // templatePool.Flush();

    // Reset OD textures
    for (const auto &od : odTextures) {
        od.second->index = 0;
    }

    // Clear states
    resources.Clear();
    textureDescs.Clear();
    textureViewDescs.Clear();

    // OK
    return COM::kOK;
}

COM::Result Scheduler::AnalyzeMutableResourceLifetime(Scheduler::FrameData *) {

    // OK
    return COM::kOK;
}

COM::Result Scheduler::AllocateResources(QueueBucket *bucket, Scheduler::FrameData *frame) {
    for (auto &import : context.resources) {
        switch (import.command.type) {
            default:
                break;
            case GHA::ResourceCommandType::eNone:
                break;
            case GHA::ResourceCommandType::eAddTexture: {
                auto &desc = odTextures[import.command.addTexture.desc.Hash()];
                if (!desc) {
                    desc = odTexturePool.Pop();
                    desc->index = 0;
                    desc->objects.Clear();
                }
                if (desc->index >= desc->objects.Size()) {
                    // Lifetime managed internally
                    GHA::ITexture *_texture;
                    if (!device->CreateTexture(nullptr, import.command.addTexture.desc, &_texture)) {
                        return COM::kError;
                    }

                    ODTexture::Object obj;
                    obj.texture = reinterpret_cast<ITexture *>(_texture);
                    desc->objects.Add(obj);
                }

                auto &obj = desc->objects[desc->index++];
                obj.texture->AcquireUsage();
                frame->memoryResources.Add(obj.texture);
                obj.gcCounter = 0;
                ImportTexture(obj.texture, import.command.addTexture.resource);
                break;
            }
            case GHA::ResourceCommandType::eImportVertexBuffer: {
                // Attempt to acquire the resource
                if (!import.command.importVertexBuffer.buffer->AcquireUsage()) {
                    WarningLog(this).Write("Skipped block due to resource ", reinterpret_cast<USize>(import.command.importVertexBuffer.buffer));
                    failedNodes.Add(import.node);
                    continue;
                }
                if (!import.command.importVertexBuffer.buffer->IsInitialized()) {
                    WarningLog(this).Write("Skipped block due to resource ", reinterpret_cast<USize>(import.command.importVertexBuffer.buffer));
                    import.command.importVertexBuffer.buffer->RemoveUsage();
                    failedNodes.Add(import.node);
                    continue;
                }

                // Process import
                frame->memoryResources.Add(import.command.importVertexBuffer.buffer);
                resources.Add(ResourceCommand::ImportVertexBuffer(import.command.importVertexBuffer.buffer, import.command.importVertexBuffer.resource));
                break;
            }
            case GHA::ResourceCommandType::eImportIndexBuffer: {
                // Attempt to acquire the resource
                if (!import.command.importIndexBuffer.buffer->AcquireUsage()) {
                    WarningLog(this).Write("Skipped block due to resource ", reinterpret_cast<USize>(import.command.importIndexBuffer.buffer));
                    failedNodes.Add(import.node);
                    continue;
                }
                if (!import.command.importIndexBuffer.buffer->IsInitialized()) {
                    WarningLog(this).Write("Skipped block due to resource ", reinterpret_cast<USize>(import.command.importIndexBuffer.buffer));
                    import.command.importIndexBuffer.buffer->RemoveUsage();
                    failedNodes.Add(import.node);
                    continue;
                }

                // Process import
                resources.Add(ResourceCommand::ImportIndexBuffer(import.command.importIndexBuffer.buffer, import.command.importIndexBuffer.resource));
                break;
            }
            case GHA::ResourceCommandType::eImportTexture: {
                // Uninitialized whilst compressed formats need special care
                if ((!import.command.importTexture.texture->IsInitialized() && IsCompressed(import.command.importTexture.texture->GetDesc().format)) ||
                    !import.command.importTexture.texture->AcquireUsage()) {
                    // Override with temporary 1x1
                    TextureViewID textureViewID = CreateTextureView(ImportTexture(dummyTexture, import.command.importTexture.resource), {});

                    Barrier(bucket, textureViewID, TextureLayout::eTransferDst);

                    bucket->commands.Add(Command::CmdClearColor(textureViewID));
                } else {
                    frame->memoryResources.Add(import.command.importTexture.texture);

                    TextureID textureID = ImportTexture(import.command.importTexture.texture, import.command.importTexture.resource);

                    if (!import.command.importTexture.texture->IsInitialized()) {
                        import.command.importTexture.texture->MarkAllAsInitialized();

                        TextureViewID textureViewID = CreateTextureView(textureID, {});

                        Barrier(bucket, textureViewID, TextureLayout::eTransferDst);

                        bucket->commands.Add(Command::CmdClearColor(textureViewID));
                    }
                }
                break;
            }
            case GHA::ResourceCommandType::eImportTextureStreamable: {
                if (import.command.importTextureStreamable.streamables.Size() == 0) {
                    TextureViewID textureViewID = CreateTextureView(ImportTexture(dummyTexture, import.command.importTextureStreamable.resource), {});
                    Barrier(bucket, textureViewID, TextureLayout::eTransferDst);
                    bucket->commands.Add(Command::CmdClearColor(textureViewID));
                    continue;
                }

                auto &desc = streamedTextures[import.command.importTextureStreamable.uid];
                if (!desc.texture) {
                    // Lifetime managed internally
                    GHA::ITexture *_texture;
                    if (!device->CreateTexture(nullptr, import.command.importTextureStreamable.desc, &_texture)) {
                        return COM::kError;
                    }

                    desc.texture = reinterpret_cast<ITexture *>(_texture);
                }

                // ...
                USize storedCommits = desc.commitIndices.Size();
                desc.commitIndices.Resize(import.command.importTextureStreamable.streamables.Size());

                // Verify that all mips are aligned with the current commit indices
                for (USize i = 0; i < import.command.importTextureStreamable.streamables.Size(); i++) {
                    auto &stream = import.command.importTextureStreamable.streamables[i];

                    // Needs update?
                    if (storedCommits <= i || stream->GetCommitIndex() != desc.commitIndices[i]) {
                        // Enumerate mip
                        USize size;
                        void *blob;
                        if (!stream->Enumerate(size, &blob)) {
                            return COM::kError;
                        }

                        // Stage mip
                        stager->StageTextureMip(desc.texture, static_cast<UInt32>(i), blob, MemoryBounds(size));

                        // Push commit
                        desc.commitIndices[i] = stream->GetCommitIndex();
                    }
                }

                // Import
                if (!desc.texture->IsInitialized()) {
                    TextureViewID textureViewID = CreateTextureView(ImportTexture(dummyTexture, import.command.importTextureStreamable.resource), {});

                    Barrier(bucket, textureViewID, TextureLayout::eTransferDst);

                    bucket->commands.Add(Command::CmdClearColor(textureViewID));
                } else {
                    ImportTexture(desc.texture, import.command.importTextureStreamable.resource);
                }
                break;
            }
            case GHA::ResourceCommandType::eImportVertexStreamable: {
                if (!import.command.importVertexStreamable.vertexStreamable) {
                    resources.Add(ResourceCommand::ImportVertexBuffer(
                            dummyVertexBuffer,
                            import.command.importVertexStreamable.vertexResource
                    ));
                    continue;
                }

                // Enumerate mip
                USize size;
                void *blob;
                if (!import.command.importVertexStreamable.vertexStreamable->Enumerate(size, &blob)) {
                    return COM::kError;
                }

                import.command.importVertexStreamable.vertexDesc.hostVisible |= (import.command.importVertexStreamable.highFrequency && size > 65536);

                auto &desc = streamedVertices[import.command.importVertexStreamable.uid];
                if ((desc.buffers.Size() == 0) || (desc.size < size)) {
                    desc.size = size;
                    import.command.importVertexStreamable.vertexDesc.count = static_cast<USize>(
                            static_cast<double>(size) / import.command.importVertexStreamable.vertexDesc.stride
                    );
                    if (!desc.buffers.Empty()) {

                    }

                    // Append to deferred deletions
                    for (auto& buffer : desc.buffers) {
                        frame->deferredDeletes.Add(buffer);
                    }

                    if (!import.command.importVertexStreamable.vertexDesc.hostVisible) {
                        // Lifetime managed internally
                        desc.buffers.Resize(1);
                        if (!device->CreateVertexBuffer(nullptr, import.command.importVertexStreamable.vertexDesc, &desc.buffers[0])) {
                            return COM::kError;
                        }
                    } else {
                        // Lifetime managed internally
                        desc.buffers.Resize(Math::Max(2u, swapchain->GetDesc().backbufferCount.Get() + 1));
                        for (UInt32 i = 0; i < desc.buffers.Size(); i++) {
                            if (!device->CreateVertexBuffer(nullptr, import.command.importVertexStreamable.vertexDesc, &desc.buffers[i])) {
                                return COM::kError;
                            }
                        }
                    }
                }

                auto bufferIndex = desc.NextIndex();

                if (!desc.initialized || desc.commitIndex != import.command.importVertexStreamable.vertexStreamable->GetCommitIndex()) {
                    if (import.command.importVertexStreamable.highFrequency) {
                        if (!import.command.importVertexStreamable.vertexDesc.hostVisible) {
                            desc.buffers[0]->MarkAllAsInitialized();

                            bucket->commands.Add(Command::CmdUpdateBuffer(
                                    import.command.importVertexStreamable.vertexResource,
                                    blob,
                                    MemoryBounds(size)
                            ));
                        } else {
                            auto buffer = desc.buffers[bufferIndex];

                            void *mapped;
                            if (!buffer->Map(&mapped)) {
                                return COM::kError;
                            }
                            Core::Memory::Copy(mapped, blob, size);
                            if (!buffer->Unmap(mapped)) {
                                return COM::kError;
                            }
                            buffer->MarkAllAsInitialized();
                        }
                    } else {
                        // Stage buffer
                        stager->StageBuffer(desc.buffers[0], blob, MemoryBounds(size));
                    }

                    // Push commit
                    desc.initialized = true;
                    desc.commitIndex = import.command.importVertexStreamable.vertexStreamable->GetCommitIndex();
                }

                if (!import.command.importVertexStreamable.vertexDesc.hostVisible) {
                    if (!desc.buffers[0]->IsInitialized()) {
                        resources.Add(ResourceCommand::ImportVertexBuffer(dummyVertexBuffer, import.command.importVertexStreamable.vertexResource));
                    } else {
                        resources.Add(ResourceCommand::ImportVertexBuffer(desc.buffers[0], import.command.importVertexStreamable.vertexResource));
                    }
                } else {
                    resources.Add(ResourceCommand::ImportVertexBuffer(
                            desc.buffers[bufferIndex],
                            import.command.importVertexStreamable.vertexResource
                    ));
                }
                break;
            }
            case GHA::ResourceCommandType::eImportIndexStreamable: {
                if (!import.command.importIndexStreamable.indexStreamable) {
                    resources.Add(ResourceCommand::ImportIndexBuffer(
                            dummyIndexBuffer,
                            import.command.importIndexStreamable.indexResource
                    ));
                    continue;
                }

                // Enumerate mip
                USize size;
                void *blob;
                if (!import.command.importIndexStreamable.indexStreamable->Enumerate(size, &blob)) {
                    return COM::kError;
                }

                import.command.importIndexStreamable.indexDesc.hostVisible |= (import.command.importIndexStreamable.highFrequency && size > 65536);

                auto &desc = streamedIndices[import.command.importIndexStreamable.uid];
                if (desc.buffers.Size() == 0 || desc.size < size) {
                    desc.size = size;
                    import.command.importIndexStreamable.indexDesc.count = static_cast<USize>(
                            static_cast<double>(size) / static_cast<UInt32>(import.command.importIndexStreamable.indexDesc.type)
                    );

                    // Append to deferred deletions
                    for (auto& buffer : desc.buffers) {
                        frame->deferredDeletes.Add(buffer);
                    }

                    if (!import.command.importIndexStreamable.indexDesc.hostVisible) {
                        // Lifetime managed internally
                        desc.buffers.Resize(1);
                        if (!device->CreateIndexBuffer(nullptr, import.command.importIndexStreamable.indexDesc, &desc.buffers[0])) {
                            return COM::kError;
                        }
                    } else {
                        // Lifetime managed internally
                        desc.buffers.Resize(Math::Max(2u, swapchain->GetDesc().backbufferCount.Get() + 1));
                        for (UInt32 i = 0; i < desc.buffers.Size(); i++) {
                            if (!device->CreateIndexBuffer(nullptr, import.command.importIndexStreamable.indexDesc, &desc.buffers[i])) {
                                return COM::kError;
                            }
                        }
                    }
                }

                auto bufferIndex = desc.NextIndex();

                if (!desc.initialized || desc.commitIndex != import.command.importIndexStreamable.indexStreamable->GetCommitIndex()) {
                    if (import.command.importIndexStreamable.highFrequency) {
                        if (!import.command.importIndexStreamable.indexDesc.hostVisible) {
                            desc.buffers[0]->MarkAllAsInitialized();

                            bucket->commands.Add(Command::CmdUpdateBuffer(
                                    import.command.importIndexStreamable.indexResource,
                                    blob,
                                    MemoryBounds(size)
                            ));
                        } else {
                            auto buffer = desc.buffers[bufferIndex];

                            void *mapped;
                            if (!buffer->Map(&mapped)) {
                                return COM::kError;
                            }
                            Core::Memory::Copy(mapped, blob, size);
                            if (!buffer->Unmap(mapped)) {
                                return COM::kError;
                            }
                            buffer->MarkAllAsInitialized();
                        }
                    } else {
                        // Stage buffer
                        stager->StageBuffer(desc.buffers[0], blob, MemoryBounds(size));
                    }

                    // Push commit
                    desc.initialized = true;
                    desc.commitIndex = import.command.importIndexStreamable.indexStreamable->GetCommitIndex();
                }

                if (!import.command.importIndexStreamable.indexDesc.hostVisible) {
                    if (!desc.buffers[0]->IsInitialized()) {
                        resources.Add(ResourceCommand::ImportIndexBuffer(dummyIndexBuffer, import.command.importIndexStreamable.indexResource));
                    } else {
                        resources.Add(ResourceCommand::ImportIndexBuffer(desc.buffers[0], import.command.importIndexStreamable.indexResource));
                    }
                } else {
                    resources.Add(ResourceCommand::ImportIndexBuffer(
                            desc.buffers[bufferIndex],
                            import.command.importIndexStreamable.indexResource
                    ));
                }
                break;
            }
        }
    }

    // OK
    return COM::kOK;
}

COM::Result Scheduler::GenerateCommands(Scheduler::FrameData *frame) {
    // Flush
    sortedNodes.Clear();
    completedNodes.Clear();

    // Resolve dependencies
    bool mutated;
    do {
        mutated = false;

        for (USize i = 0; i < scheduledNodes.Size(); i++) {
            // Verify that all the dependencies have been met, skip if failed lock
            if (scheduledNodes[i]->dependencies.All([&](GHA::ScheduledNode *dep) {
                if (failedNodes.Contains(dep)) {
                    failedNodes.Add(scheduledNodes[i]);
                }
                return completedNodes.Contains(dep);
            }) && !failedNodes.Contains(scheduledNodes[i])) {
                completedNodes.Add(scheduledNodes[i]);
                sortedNodes.Add(scheduledNodes[i]);
                scheduledNodes.Remove(i);
                i--;
                mutated = true;
            }

                // Remove from graph if failed lock
            else if (failedNodes.Contains(scheduledNodes[i])) {
                scheduledNodes.Remove(i);
                i--;
                mutated = true;
            }
        }
    } while (mutated);

    // Resolved?
    if (scheduledNodes.Size() != 0) {
        ErrorLog(this).Write("Circular scheduled nodes, cannot resolve");
        return COM::kOK;
    }

    // Current backbuffer
    auto backbuffer = swapchain->GetTexture();
    backbuffer->AcquireUsage();

    // Prepare backbuffer
    TextureViewID backbufferViewID = CreateTextureView(ImportTexture(backbuffer), {});

    // Compile nodes
    for (auto node : sortedNodes) {
        if (COM::CheckedResult result = CompileNode(frame, &queues[0], node)) {
            return result;
        }
    }

    // Transition to present src
    Barrier(&queues[0], backbufferViewID, TextureLayout::ePresentSrc);

    // Cleanup
    scheduledNodes.Clear();

    // OK
    return COM::kOK;
}

TextureID Scheduler::ImportTexture(GHA::ITexture *texture) {
    TextureID uid = {context.CreateUID()};
    resources.Add(ResourceCommand::ImportTexture(texture, uid));
    IntermediateTextureDesc interm;
    interm.desc = texture->GetDesc();
    interm.texture = reinterpret_cast<PipelineScheduler::ITexture *>(texture);
    textureDescs[uid.Get()] = interm;
    return uid;
}

TextureViewID Scheduler::CreateTextureView(TextureID id, const TextureViewDesc &desc) {
    DJINN_ASSERT(textureDescs[id.Get()].texture != nullptr, "Invalid texture");
    TextureViewID uid = {context.CreateUID()};
    resources.Add(ResourceCommand::CreateTextureView(desc, id, uid));
    textureViewDescs[uid.Get()] = {textureDescs[id.Get()].texture, id, desc};
    return uid;
}

TextureID Scheduler::ImportTexture(GHA::ITexture *texture, TextureID uid) {
    DJINN_ASSERT(texture != nullptr, "Invalid texture");
    resources.Add(ResourceCommand::ImportTexture(texture, uid));
    IntermediateTextureDesc interm;
    interm.desc = texture->GetDesc();
    interm.texture = reinterpret_cast<PipelineScheduler::ITexture *>(texture);
    textureDescs[uid.Get()] = interm;
    return uid;
}

COM::Result Scheduler::GarbageCollect(Scheduler::FrameData *frame) {
    // Collect OD Textures
    for (const auto &od : odTextures) {
        od.second->objects.RemoveAll([&](ODTexture::Object &obj) {
            if (static_cast<UInt32>(obj.gcCounter) >= gcCounterTreshold) {
                if (ITexture *&ms = textureMs[obj.texture]) {
                    if (ms->GetUsageCount() == 0) {
                        if (COM::CheckedResult result = ms->Release()) {
                            ErrorLog(this).Write("Failed to release handle");
                        }
                        ms = nullptr;
                    } else {
                        ErrorLog(this).Write("GPU Garbage collector expects no usages on scheduled multisample resource");
                    }
                }

                if (obj.texture->GetUsageCount() == 0) {
                    if (COM::CheckedResult result = obj.texture->Release()) {
                        ErrorLog(this).Write("Failed to release handle");
                    }
                    obj.texture = nullptr;
                } else {
                    ErrorLog(this).Write("GPU Garbage collector expects no usages on scheduled resource");
                }
                return true;
            }
            obj.gcCounter++;
            return false;
        });
    }

    // OK
    return COM::kOK;
}

PersistentID Scheduler::CreatePersistent(ResourceID id) {
    PersistentResource resource;
    PersistentID persistent = {persistentResources.Add(resource)};
    resources.Add(ResourceCommand::StorePersistent(persistent, id));
    return persistent;
}

ResourceID Scheduler::LoadPersistent(PersistentID persistent) {
    auto id = ResourceID(context.CreateUID());
    resources.Add(ResourceCommand::LoadPersistent(persistent, id));
    return id;
}

COM::Result Scheduler::Released() {
    // Take care of deferred deletions
    for (auto& frame : frames) {
        for (auto& res : frame.deferredDeletes) {
            Check(res->Release());
        }
    }

    // Release constant buffers
    for (auto kv : constantBuffers) Check(kv.second->Release());

    // Release shaders
    for (auto kv : shaders) Check(kv.second->Release());

    // Release ms textures
    for (auto kv : textureMs) if (kv.second) Check(kv.second->Release());

    // Release od textures
    for (auto kv : odTextures) for (auto obj : kv.second->objects) Check(obj.texture->Release());

    // Release streamed data
    for (auto kv : streamedTextures) Check(kv.second.texture->Release());
    for (auto kv : streamedIndices) for (auto buffer : kv.second.buffers) Check(buffer->Release());
    for (auto kv : streamedVertices) for (auto buffer : kv.second.buffers) Check(buffer->Release());

    // OK
    return COM::kOK;
}
