//
// Created by Dev on 12/24/2017.
//
#pragma once

#include <Core/Memory.h>
#include <GHA/TextureDesc.h>
#include <GHA/Viewport.h>
#include <GHA/TextureViewDesc.h>
#include <GHA/BindPoint.h>
#include "CommandTypes.h"
#include "PipelineLayoutDesc.h"
#include "PipelineDesc.h"
#include "PassDesc.h"
#include "FrameDesc.h"
#include "DescriptorSetLayoutDesc.h"
#include "DescriptorSetUpdateDesc.h"
#include "CommandListDesc.h"
#include "CommandPoolDesc.h"
#include "ID.h"
#include <Core/FixedArray.h>

namespace Djinn::GHA {
    class ITexture;
    class IVertexBuffer;
    class IIndexBuffer;
    class IConstantBuffer;

    namespace PipelineScheduler {
        struct ResourceCommand {
            static ResourceCommand ImportTexture(GHA::ITexture *texture, TextureID uid) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eImportTexture;
                cmd.payload.importTexture.texture = texture;
                cmd.payload.importTexture.uid = uid;
                return cmd;
            }

            static ResourceCommand ImportVertexBuffer(GHA::IVertexBuffer *buffer, ResourceID uid) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eImportVertexBuffer;
                cmd.payload.importVertexBuffer.buffer = buffer;
                cmd.payload.importVertexBuffer.uid = uid;
                return cmd;
            }

            static ResourceCommand ImportIndexBuffer(GHA::IIndexBuffer *buffer, ResourceID uid) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eImportIndexBuffer;
                cmd.payload.importIndexBuffer.buffer = buffer;
                cmd.payload.importIndexBuffer.uid = uid;
                return cmd;
            }

            static ResourceCommand ImportConstantBuffer(GHA::IConstantBuffer *buffer, ResourceID uid) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eImportConstantBuffer;
                cmd.payload.importConstantBuffer.buffer = buffer;
                cmd.payload.importConstantBuffer.uid = uid;
                return cmd;
            }

            static ResourceCommand CreateTextureView(const TextureViewDesc& desc, TextureID texture, TextureViewID uid) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreateTextureView;
                cmd.payload.createTextureView.texture = texture;
                cmd.payload.createTextureView.desc = desc;
                cmd.payload.createTextureView.uid = uid;
                return cmd;
            }

            static ResourceCommand CreateSampler(const SamplerDesc& desc, SamplerID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreateSampler;
                cmd.payload.createSampler.uid = id;
                cmd.payload.createSampler.desc = desc;
                return cmd;
            }

            static ResourceCommand CreateVertexBufferView(ResourceID vertexBuffer, ResourceID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreateVertexBufferView;
                cmd.payload.createVertexBufferView.uid = id;
                cmd.payload.createVertexBufferView.vertexBuffer = vertexBuffer;
                return cmd;
            }

            static ResourceCommand CreateIndexBufferView(ResourceID indexBuffer, ResourceID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreateIndexBufferView;
                cmd.payload.createIndexBufferView.uid = id;
                cmd.payload.createIndexBufferView.indexBuffer = indexBuffer;
                return cmd;
            }

            static ResourceCommand CreateStructuredBufferView(ResourceID structuredBuffer, ResourceID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreateStructuredBufferView;
                cmd.payload.createStructuredBufferView.uid = id;
                cmd.payload.createStructuredBufferView.structuredBuffer = structuredBuffer;
                return cmd;
            }

            static ResourceCommand CreateLinearBufferView(ResourceID linearBuffer, ResourceID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreateLinearBufferView;
                cmd.payload.createLinearBufferView.uid = id;
                cmd.payload.createLinearBufferView.linearBuffer = linearBuffer;
                return cmd;
            }

            static ResourceCommand CreateTextureUnorderedView(TextureViewID view, TextureUAVID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreateTextureUnorderedView;
                cmd.payload.createTextureUnorderedView.uid = id;
                cmd.payload.createTextureUnorderedView.textureView = view;
                return cmd;
            }

            static ResourceCommand CreateVertexBufferUnorderedView(ResourceID view, ResourceID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreateVertexBufferUnorderedView;
                cmd.payload.createVertexBufferUnorderedView.uid = id;
                cmd.payload.createVertexBufferUnorderedView.vertexBufferView = view;
                return cmd;
            }

            static ResourceCommand CreateIndexBufferUnorderedView(ResourceID view, ResourceID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreateIndexBufferUnorderedView;
                cmd.payload.createIndexBufferUnorderedView.uid = id;
                cmd.payload.createIndexBufferUnorderedView.indexBufferView = view;
                return cmd;
            }

            static ResourceCommand CreateStructuredBufferUnorderedView(ResourceID view, ResourceID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreateStructuredBufferUnorderedView;
                cmd.payload.createStructuredBufferUnorderedView.uid = id;
                cmd.payload.createStructuredBufferUnorderedView.structuredBufferView = view;
                return cmd;
            }

            static ResourceCommand CreateLinearBufferUnorderedView(ResourceID view, ResourceID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreateLinearBufferUnorderedView;
                cmd.payload.createLinearBufferUnorderedView.uid = id;
                cmd.payload.createLinearBufferUnorderedView.linearBufferView = view;
                return cmd;
            }

            static ResourceCommand CreatePipeline(PipelineDesc* desc, ResourceID layout, ResourceID pass, ResourceID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreatePipeline;
                cmd.payload.createPipeline.uid = id;
                cmd.payload.createPipeline.desc = desc;
                cmd.payload.createPipeline.layout = layout;
                cmd.payload.createPipeline.pass = pass;
                return cmd;
            }

            static ResourceCommand CreatePipelineLayout(PipelineLayoutDesc desc, ResourceID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreatePipelineLayout;
                cmd.payload.createPipelineLayout.uid = id;
                cmd.payload.createPipelineLayout.desc = desc;
                return cmd;
            }

            static ResourceCommand CreateDescriptorSetLayout(DescriptorSetLayoutDesc* desc, ResourceID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreateDescriptorSetLayout;
                cmd.payload.createDescriptorSetLayout.uid = id;
                cmd.payload.createDescriptorSetLayout.desc = desc;
                return cmd;
            }

            static ResourceCommand CreateDescriptorSet(DescriptorSetUpdateDesc* desc, ResourceID layout, ResourceID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreateDescriptorSet;
                cmd.payload.createDescriptorSet.uid = id;
                cmd.payload.createDescriptorSet.layout = layout;
                cmd.payload.createDescriptorSet.desc = desc;
                return cmd;
            }

            static ResourceCommand CreatePass(PassDesc* desc, PassID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreatePass;
                cmd.payload.createPass.uid = id;
                cmd.payload.createPass.desc = desc;
                return cmd;
            }

            static ResourceCommand CreateFrame(FrameDesc* desc, PassID pid, FrameID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreateFrame;
                cmd.payload.createFrame.uid = id;
                cmd.payload.createFrame.pass = pid;
                cmd.payload.createFrame.desc = desc;
                return cmd;
            }

            static ResourceCommand CreateCommandList(CommandListDesc desc, ResourceID pool, ResourceID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreateCommandList;
                cmd.payload.commandList.uid = id;
                cmd.payload.commandList.pool = pool;
                cmd.payload.commandList.desc = desc;
                return cmd;
            }

            static ResourceCommand CreateCommandPool(CommandPoolDesc desc, ResourceID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eCreateCommandPool;
                cmd.payload.commandPool.uid = id;
                cmd.payload.commandPool.desc = desc;
                return cmd;
            }

            static ResourceCommand UpdateDescriptorSet(ResourceID descriptorSet, DescriptorSetUpdateDesc* desc) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eUpdateDescriptorSet;
                cmd.payload.updateDescriptorSet.descriptorSet = descriptorSet;
                cmd.payload.updateDescriptorSet.desc = desc;
                return cmd;
            }

            static ResourceCommand StorePersistent(PersistentID persistent, ResourceID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eStorePersistent;
                cmd.payload.storePersistent.persistent = persistent;
                cmd.payload.storePersistent.id = id;
                return cmd;
            }

            static ResourceCommand LoadPersistent(PersistentID persistent, ResourceID id) {
                ResourceCommand cmd;
                cmd.type = ResourceCommandType::eLoadPersistent;
                cmd.payload.loadPersistent.persistent = persistent;
                cmd.payload.loadPersistent.id = id;
                return cmd;
            }

            /// Set hash mode
            /// \param b
            /// \return
            ResourceCommand& Hashed(bool b) {
                isHashed = b;
                return *this;
            }

            /// Command payload
            union Payload {
                Payload() {

                }

                /// Initialize copy
                /// \param other
                Payload(const Payload &other) {
                    Core::Memory::Copy(this, &other, sizeof(Payload));
                }

                /// Assign from other payload
                /// \param other
                /// \return
                Payload &operator=(const Payload &other) {
                    Core::Memory::Copy(this, &other, sizeof(Payload));
                    return *this;
                }

                struct ImportTexture {
                    GHA::ITexture *texture;
                    TextureID uid;
                } importTexture;

                struct ImportVertexBuffer {
                    GHA::IVertexBuffer *buffer;
                    ResourceID uid;
                } importVertexBuffer;

                struct ImportIndexBuffer {
                    GHA::IIndexBuffer *buffer;
                    ResourceID uid;
                } importIndexBuffer;

                struct ImportConstantBuffer {
                    GHA::IConstantBuffer *buffer;
                    ResourceID uid;
                } importConstantBuffer;

                struct CreateTextureView {
                    TextureViewDesc desc;
                    TextureID texture;
                    TextureViewID uid;
                } createTextureView;

                struct CreateSampler {
                    SamplerDesc desc;
                    SamplerID uid;
                } createSampler;

                struct CreateVertexBufferView {
                    ResourceID vertexBuffer;
                    ResourceID uid;
                } createVertexBufferView;

                struct CreateIndexBufferView {
                    ResourceID indexBuffer;
                    ResourceID uid;
                } createIndexBufferView;

                struct CreateStructuredBufferView {
                    ResourceID structuredBuffer;
                    ResourceID uid;
                } createStructuredBufferView;

                struct CreateLinearBufferView {
                    ResourceID linearBuffer;
                    ResourceID uid;
                } createLinearBufferView;

                struct CreateTextureUnorderedView {
                    TextureViewID textureView;
                    TextureUAVID uid;
                } createTextureUnorderedView;

                struct CreateVertexBufferUnorderedView {
                    ResourceID vertexBufferView;
                    ResourceID uid;
                } createVertexBufferUnorderedView;

                struct CreateIndexBufferUnorderedView {
                    ResourceID indexBufferView;
                    ResourceID uid;
                } createIndexBufferUnorderedView;

                struct CreateStructuredBufferUnorderedView {
                    ResourceID structuredBufferView;
                    ResourceID uid;
                } createStructuredBufferUnorderedView;

                struct CreateLinearBufferUnorderedView {
                    ResourceID linearBufferView;
                    ResourceID uid;
                } createLinearBufferUnorderedView;

                struct CreatePipeline {
                    PipelineDesc* desc;
                    ResourceID layout;
                    ResourceID pass;
                    ResourceID uid;
                } createPipeline;

                struct CreatePipelineLayout {
                    PipelineLayoutDesc desc;
                    ResourceID uid;
                } createPipelineLayout;

                struct CreateDescriptorSetLayout {
                    DescriptorSetLayoutDesc* desc;
                    ResourceID uid;
                } createDescriptorSetLayout;

                struct CreateDescriptorSet {
                    DescriptorSetUpdateDesc* desc;
                    ResourceID layout;
                    ResourceID uid;
                } createDescriptorSet;

                struct CreatePass {
                    PassDesc* desc;
                    PassID uid;
                } createPass;

                struct CreateFrame {
                    FrameDesc* desc;
                    PassID pass;
                    FrameID uid;
                } createFrame;

                struct CreateCommandList {
                    CommandListDesc desc;
                    ResourceID pool;
                    ResourceID uid;
                } commandList;

                struct CreateCommandPool {
                    CommandPoolDesc desc;
                    ResourceID uid;
                } commandPool;

                struct UpdateDescriptorSet {
                    ResourceID descriptorSet;
                    DescriptorSetUpdateDesc* desc;
                } updateDescriptorSet;

                struct StorePersistent {
                    PersistentID persistent;
                    ResourceID id;
                } storePersistent;

                struct LoadPersistent {
                    PersistentID persistent;
                    ResourceID id;
                } loadPersistent;
            } payload;

            /// Type of this command
            ResourceCommandType type = ResourceCommandType::eNone;

            /// Supports use of hashed checks?
            bool isHashed = true;
        };

        union Command {
            /// Initialize empty command
            Command() : type(CommandTypes::eNone) {
                // ...
            }

            /// Initialize copy
            /// \param other
            Command(const Command &other) {
                Core::Memory::Copy(this, &other, sizeof(Command));
            }

            /// Assign from other command
            /// \param other
            /// \return
            Command &operator=(const Command &other) {
                Core::Memory::Copy(this, &other, sizeof(Command));
                return *this;
            }

            /// See CommandType.h for details
            struct CmdDebugPushEvent {
                CmdDebugPushEvent() = default;
                CmdDebugPushEvent(const char* name) : name(name) {
                    // ...
                }

                CommandTypes type = CommandTypes::eCmdDebugPushEvent;
                const char *name;
            } cmdDebugPushEvent;

            /// Construct
            Command(const CmdDebugPushEvent &value) {
                cmdDebugPushEvent = value;
            }

            /// See CommandType.h for details
            struct CmdDebugPopEvent {
                CmdDebugPopEvent() = default;

                CommandTypes type = CommandTypes::eCmdDebugPopEvent;
            } cmdDebugPopEvent;

            /// Construct
            Command(const CmdDebugPopEvent &value) {
                cmdDebugPopEvent = value;
            }

            /// See CommandType.h for details
            struct CmdCopyTexture {
                CmdCopyTexture() = default;
                CmdCopyTexture(TextureViewID source, TextureViewID dest) : source(source), dest(dest) {
                    // ...
                }

                CommandTypes type = CommandTypes::eCmdCopyTexture;
                TextureViewID source;
                TextureViewID dest;
            } cmdCopyTexture;

            /// Construct
            Command(const CmdCopyTexture &value) {
                cmdCopyTexture = value;
            }

            /// See CommandType.h for details
            struct CmdBeginPass {
                CmdBeginPass() = default;
                CmdBeginPass(FrameID frame, PassID pass, const Rect& rect) : frame(frame), pass(pass), rect(rect) {
                        // ...
                }

                CommandTypes type = CommandTypes::eCmdBeginPass;
                FrameID frame;
                PassID pass;
                Rect rect;
            } cmdBeginPass;

            /// Construct
            Command(const CmdBeginPass &value) {
                cmdBeginPass = value;
            }

            /// See CommandType.h for details
            struct CmdEndPass {
                CmdEndPass() = default;

                CommandTypes type = CommandTypes::eCmdEndPass;
            } cmdEndPass;

            /// Construct
            Command(const CmdEndPass &value) {
                cmdEndPass = value;
            }

            /// See CommandType.h for details
            struct CmdClear {
                CmdClear() = default;
                CmdClear(ResourceID resource) : resource(resource) {
                        // ...
                }

                CommandTypes type = CommandTypes::eCmdClear;
                ResourceID resource;
            } cmdClear;

            /// Construct
            Command(const CmdClear &value) {
                cmdClear = value;
            }

            /// See CommandType.h for details
            struct CmdClearColor {
                CmdClearColor() = default;
                CmdClearColor(TextureViewID resource) : resource(resource) {
                        // ...
                }

                CommandTypes type = CommandTypes::eCmdClearColor;
                TextureViewID resource;
            } cmdClearColor;

            /// Construct
            Command(const CmdClearColor &value) {
                cmdClearColor = value;
            }

            /// See CommandType.h for details
            struct CmdClearDepthStencil {
                CmdClearDepthStencil() = default;
                CmdClearDepthStencil(TextureViewID resource) : resource(resource) {
                        // ...
                }

                CommandTypes type = CommandTypes::eCmdClearDepthStencil;
                TextureViewID resource;
            } cmdClearDepthStencil;

            /// Construct
            Command(const CmdClearDepthStencil &value) {
                cmdClearDepthStencil = value;
            }

            /// See CommandType.h for details
            struct CmdBindPipeline {
                CmdBindPipeline() = default;
                CmdBindPipeline(ResourceID pipeline) : pipeline(pipeline) {
                    // ...
                }

                CommandTypes type = CommandTypes::eCmdBindPipeline;
                ResourceID pipeline;
            } cmdBindPipeline;

            /// Construct
            Command(const CmdBindPipeline &value) {
                cmdBindPipeline = value;
            }

            /// See CommandType.h for details
            struct CmdBindDescriptorSet {
                CmdBindDescriptorSet() = default;
                CmdBindDescriptorSet(ResourceID descriptorSet, ResourceID pipelineLayoutID, GHA::BindPoint bindPoint, UInt32 index)
                            : descriptorSet(descriptorSet), pipelineLayoutID(pipelineLayoutID), bindPoint(bindPoint), index(index) {
                    // ...
                }

                CommandTypes type = CommandTypes::eCmdBindDescriptorSet;
                ResourceID descriptorSet;
                ResourceID pipelineLayoutID;
                GHA::BindPoint bindPoint;
                UInt32 index;
            } cmdBindDescriptorSet;

            /// Construct
            Command(const CmdBindDescriptorSet &value) {
                cmdBindDescriptorSet = value;
            }

            /// See CommandType.h for details
            struct CmdBindVertexBuffer {
                CmdBindVertexBuffer() = default;

                CmdBindVertexBuffer(UInt32 offset) : offset(offset) {
                    // ...
                }

                CommandTypes type = CommandTypes::eCmdBindVertexBuffers;
                UInt32 offset;
                Core::FixedArray8<ResourceID> vertexBuffers;
            } cmdBindVertexBuffer;

            /// Construct
            Command(const CmdBindVertexBuffer &value) {
                cmdBindVertexBuffer = value;
            }

            /// See CommandType.h for details
            struct CmdBindIndexBuffer {
                CmdBindIndexBuffer() = default;

                CmdBindIndexBuffer(ResourceID indexBuffer) : indexBuffer(indexBuffer) {
                    // ...
                }

                CommandTypes type = CommandTypes::eCmdBindIndexBuffer;
                ResourceID indexBuffer;
            } cmdBindIndexBuffer;

            /// Construct
            Command(const CmdBindIndexBuffer &value) {
                cmdBindIndexBuffer = value;
            }

            /// See CommandType.h for details
            struct CmdUpdateBuffer {
                CmdUpdateBuffer() = default;

                CmdUpdateBuffer(ResourceID buffer, void *data, GHA::MemoryBounds bounds) : buffer(buffer), data(data), bounds(bounds) {
                    // ...
                }

                CommandTypes type = CommandTypes::eCmdUpdateBuffer;
                ResourceID buffer;
                void *data;
                GHA::MemoryBounds bounds;
            } cmdUpdateConstantBuffer;

            /// Construct
            Command(const CmdUpdateBuffer &value) {
                cmdUpdateConstantBuffer = value;
            }

            /// See CommandType.h for details
            struct CmdUpdateDynamic {
                CmdUpdateDynamic() = default;

                CmdUpdateDynamic(ResourceID layout, GHA::StageSet visibleStages, void *data, GHA::MemoryBounds bounds) : layout(layout), visibleStages(visibleStages), data(data), bounds(bounds) {
                    // ...
                }

                CommandTypes type = CommandTypes::eCmdUpdateDynamic;
                ResourceID layout;
                GHA::StageSet visibleStages;
                void *data;
                GHA::MemoryBounds bounds;
            } cmdUpdateDynamic;

            /// Construct
            Command(const CmdUpdateDynamic &value) {
                cmdUpdateDynamic = value;
            }

            /// See CommandType.h for details
            struct SetViewport {
                SetViewport() = default;

                SetViewport(UInt32 offset, const Viewport& viewport) : offset(offset), viewport(viewport) {
                    // ...
                }

                CommandTypes type = CommandTypes::eCmdSetViewport;
                UInt32 offset;
                Viewport viewport;
            } setViewport;

            /// Construct
            Command(const SetViewport &value) {
                setViewport = value;
            }

            /// See CommandType.h for details
            struct SetScissor {
                SetScissor() = default;

                SetScissor(UInt32 offset, const Rect& scissor) : offset(offset), scissor(scissor) {
                    // ...
                }

                CommandTypes type = CommandTypes::eCmdSetScissor;
                UInt32 offset;
                Rect scissor;
            } setScissor;

            /// Construct
            Command(const SetScissor &value) {
                setScissor = value;
            }

            /// See CommandType.h for details
            struct CmdDraw {
                CmdDraw() = default;

                CmdDraw(UInt32 vertexCount, UInt32 instanceCount, UInt32 vertexOffset, UInt32 instanceOffset)
                        : vertexCount(vertexCount), instanceCount(instanceCount), vertexOffset(vertexOffset), instanceOffset(instanceOffset) {
                    // ...
                }

                CommandTypes type = CommandTypes::eCmdDraw;
                UInt32 vertexCount;
                UInt32 instanceCount;
                UInt32 vertexOffset;
                UInt32 instanceOffset;
            } cmdDraw;

            /// Construct
            Command(const CmdDraw &value) {
                cmdDraw = value;
            }

            /// See CommandType.h for details
            struct CmdDrawIndexed {
                CmdDrawIndexed() = default;

                CmdDrawIndexed(UInt32 indexCount, UInt32 instanceCount, UInt32 vertexOffset, UInt32 indexOffset, UInt32 instanceOffset)
                        : indexCount(indexCount), instanceCount(instanceCount), vertexOffset(vertexOffset), indexOffset(indexOffset), instanceOffset(instanceOffset) {
                    // ...
                }

                CommandTypes type = CommandTypes::eCmdDrawIndexed;
                UInt32 indexCount;
                UInt32 instanceCount;
                UInt32 vertexOffset;
                UInt32 indexOffset;
                UInt32 instanceOffset;
            } cmdDrawIndexed;

            /// Construct
            Command(const CmdDrawIndexed &value) {
                cmdDrawIndexed = value;
            }

            /// See CommandType.h for details
            struct CmdDrawIndirect {
                CmdDrawIndirect() = default;

                CmdDrawIndirect(ResourceID resource, UInt32 count, UInt32 offset, UInt32 stride) : resource(resource), count(count), offset(offset), stride(stride) {
                    // ...
                }

                CommandTypes type = CommandTypes::eCmdDrawIndirect;
                ResourceID resource;
                UInt32 count;
                UInt32 offset;
                UInt32 stride;
            } cmdDrawIndirect;

            /// Construct
            Command(const CmdDrawIndirect &value) {
                cmdDrawIndirect = value;
            }

            /// See CommandType.h for details
            struct CmdDrawIndirectIndexed {
                CmdDrawIndirectIndexed() = default;

                CmdDrawIndirectIndexed(ResourceID resource, UInt32 count, UInt32 offset, UInt32 stride) : resource(resource), count(count), offset(offset), stride(stride) {
                    // ...
                }

                CommandTypes type = CommandTypes::eCmdDrawIndirectIndexed;
                ResourceID resource;
                UInt32 count;
                UInt32 offset;
                UInt32 stride;
            } cmdDrawIndirectIndexed;

            /// Construct
            Command(const CmdDrawIndirectIndexed &value) {
                cmdDrawIndirectIndexed = value;
            }

            /// See CommandType.h for details
            struct CmdDispatch {
                CmdDispatch() = default;

                CmdDispatch(UInt32 x, UInt32 y, UInt32 z) : x(x), y(y), z(z) {
                    // ...
                }

                CommandTypes type = CommandTypes::eCmdDispatch;
                UInt32 x;
                UInt32 y;
                UInt32 z;
            } cmdDispatch;

            /// Construct
            Command(const CmdDispatch &value) {
                cmdDispatch = value;
            }

            /// See CommandType.h for details
            struct CmdDispatchIndirect {
                CmdDispatchIndirect() = default;

                CmdDispatchIndirect(ResourceID resource) : resource(resource) {
                    // ...
                }

                CommandTypes type = CommandTypes::eCmdDispatchIndirect;
                ResourceID resource;
            } cmdDispatchIndirect;

            /// Construct
            Command(const CmdDispatchIndirect &value) {
                cmdDispatchIndirect = value;
            }

            /// See CommandType.h for details
            struct CmdSignalFence {
                CmdSignalFence() = default;

                CmdSignalFence(ResourceID fence) : fence(fence) {
                    // ...
                }

                CommandTypes type = CommandTypes::eCmdSignalFence;
                ResourceID fence;
            } cmdSignalFence;

            /// Construct
            Command(const CmdSignalFence &value) {
                cmdSignalFence = value;
            }

            /// See CommandType.h for details
            struct CmdWaitFence {
                CmdWaitFence() = default;

                CmdWaitFence(ResourceID fence) : fence(fence) {
                    // ...
                }

                CommandTypes type = CommandTypes::eCmdWaitFence;
                ResourceID fence;
            } cmdWaitFence;

            /// Construct
            Command(const CmdWaitFence &value) {
                cmdWaitFence = value;
            }

            /// See CommandType.h for details
            struct CmdTextureBarrier {
                CommandTypes type = CommandTypes::eCmdTextureBarrier;
                struct Barrier {
                    Barrier() = default;
                    Barrier(TextureViewID id, const MipBounds& mipBounds, TextureLayout from, TextureLayout to) : texture(id), mipBounds(mipBounds), from(from), to(to) {
                        // ...
                    }

                    TextureViewID texture;
                    MipBounds mipBounds;
                    TextureLayout from;
                    TextureLayout to;
                };
                Core::FixedArray16<Barrier>* barriers;
            } cmdTextureBarrier;

            /// Construct
            Command(const CmdTextureBarrier &value) {
                cmdTextureBarrier = value;
            }

            // Type wrapper
            CommandTypes type;
        };
    }
}