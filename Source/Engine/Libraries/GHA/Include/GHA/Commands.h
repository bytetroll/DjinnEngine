//
// Created by Dev on 12/22/2017.
//
#pragma once

#include <Core/Memory.h>
#include "CommandTypes.h"
#include "TextureDesc.h"
#include "ID.h"
#include "BlendStateDesc.h"
#include "RasterizerStateDesc.h"
#include "DepthStencilStateDesc.h"
#include "VertexBufferDesc.h"
#include "IndexBufferDesc.h"
#include "Topology.h"
#include "MultiSampleStateDesc.h"

namespace Djinn::GHA {
    class ITexture;
    class IVertexBuffer;
    class IIndexBuffer;
    class IShader;
    class IStreamable;
    struct ShaderBlob;
    class IConstantBuffer;

    /// Resource command structure
    union ResourceCommand {
        /// Initialize empty command
        ResourceCommand() : type(ResourceCommandType::eNone) {
            // ...
        }

        /// Initialize copy
        /// \param other
        ResourceCommand(const ResourceCommand &other) {
            Core::Memory::Copy(this, &other, sizeof(ResourceCommand));
        }

        /// Assign from other command
        /// \param other
        /// \return
        ResourceCommand &operator=(const ResourceCommand &other) {
            Core::Memory::Copy(this, &other, sizeof(ResourceCommand));
            return *this;
        }

        /// See CommandType.h for details
        struct AddTexture {
            ResourceCommandType type = ResourceCommandType::eAddTexture;
            TextureDesc desc;
            TextureID resource;
        } addTexture;

        /// See CommandType.h for details
        struct ImportVertexBuffer {
            ResourceCommandType type = ResourceCommandType::eImportVertexBuffer;
            IVertexBuffer *buffer;
            VertexBufferID resource;
        } importVertexBuffer;

        /// See CommandType.h for details
        struct ImportIndexBuffer {
            ResourceCommandType type = ResourceCommandType::eImportIndexBuffer;
            IIndexBuffer *buffer;
            IndexBufferID resource;
        } importIndexBuffer;

        /// See CommandType.h for details
        struct ImportConstantBuffer {
            ResourceCommandType type = ResourceCommandType::eImportConstantBuffer;
            IConstantBuffer *buffer;
            ConstantBufferID resource;
        } importConstantBuffer;

        /// See CommandType.h for details
        struct ImportTexture {
            ResourceCommandType type = ResourceCommandType::eImportTexture;
            ITexture *texture;
            TextureID resource;
        } importTexture;

        /// See CommandType.h for details
        struct ImportTextureStreamable {
            ResourceCommandType type = ResourceCommandType::eImportTextureStreamable;
            TextureDesc desc;
            USize uid;
            Core::FixedArray16<IStreamable*> streamables;
            TextureID resource;
        } importTextureStreamable;

        /// See CommandType.h for details
        struct ImportVertexStreamable {
            ResourceCommandType type = ResourceCommandType::eImportVertexStreamable;
            USize uid;
            VertexBufferDesc vertexDesc;
            IStreamable* vertexStreamable;
            VertexBufferID vertexResource;
            bool highFrequency;
        } importVertexStreamable;

        /// See CommandType.h for details
        struct ImportIndexStreamable {
            ResourceCommandType type = ResourceCommandType::eImportIndexStreamable;
            USize uid;
            IndexBufferDesc indexDesc;
            IStreamable* indexStreamable;
            IndexBufferID indexResource;
            bool highFrequency;
        } importIndexStreamable;

        // Type wrapper
        ResourceCommandType type;
    };

    /// Abstract command structure
    union Command {
        /// Initialize empty command
        Command() : type(CommandType::eNone) {
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
        struct DebugPushEvent {
            CommandType type = CommandType::eDebugPushEvent;
            const char *name;
        } debugPushEvent;

        /// See CommandType.h for details
        struct DebugPopEvent {
            CommandType type = CommandType::eDebugPopEvent;
        } debugPopEvent;

        /// See CommandType.h for details
        struct CmdCopyTexture {
            CommandType type = CommandType::eCmdCopyTexture;
            TextureID source;
            TextureID dest;
        } cmdCopyTexture;

        /// See CommandType.h for details
        struct CmdClear {
            CommandType type = CommandType::eCmdClear;
            ResourceID resource;
        } cmdClear;

        /// See CommandType.h for details
        struct CmdClearColor {
            CommandType type = CommandType::eCmdClearColor;
            TextureID resource;
        } cmdClearColor;

        /// See CommandType.h for details
        struct CmdClearDepthStencil {
            CommandType type = CommandType::eCmdClearDepthStencil;
            TextureID resource;
        } cmdClearDepthStencil;

        /// See CommandType.h for details
        struct SetTopology {
            CommandType type = CommandType::eSetTopology;
            GHA::Topology topology;
        } setTopology;

        /// See CommandType.h for details
        struct BindRenderTarget {
            CommandType type = CommandType::eBindRenderTarget;
            TextureID texture;
            TextureViewDesc viewDesc;
            RenderTargetBlendDesc blendDesc;
            UInt32 offset;
        } bindRenderTarget;

        /// See CommandType.h for details
        struct BindDepthRenderTarget {
            CommandType type = CommandType::eBindDepthRenderTarget;
            TextureID texture;
            TextureViewDesc viewDesc;
        } bindDepthRenderTarget;

        /// See CommandType.h for details
        struct BindResource {
            CommandType type = CommandType::eBindResource;
            BindID bindID;
            ResourceID resource;
        } bindResource;

        /// See CommandType.h for details
        struct BindTexture {
            CommandType type = CommandType::eBindTexture;
            BindID bindID;
            TextureID texture;
            TextureViewDesc viewDesc;
            Int32 arrayIndex = -1;
            SamplerDesc samplerDesc;
        } bindTexture;

        /// See CommandType.h for details
        struct BindVertexBuffer {
            CommandType type = CommandType::eBindVertexBuffer;
            VertexBufferID vertexBuffer;
            UInt32 offset;
        } bindVertexBuffer;

        /// See CommandType.h for details
        struct BindIndexBuffer {
            CommandType type = CommandType::eBindIndexBuffer;
            IndexBufferID buffer;
        } bindIndexBuffer;

        /// See CommandType.h for details
        struct SetRasterizerState {
            CommandType type = CommandType::eSetRasterizerState;
            RasterizerStateDesc desc;
        } setRasterizerState;

        /// See CommandType.h for details
        struct SetDepthStencilState {
            CommandType type = CommandType::eSetDepthStencilState;
            DepthStencilStateDesc desc;
        } setDepthStencilState;

        /// See CommandType.h for details
        struct SetViewport {
            CommandType type = CommandType::eSetViewport;
            UInt32 offset;
            Viewport viewport;
        } setViewport;

        /// See CommandType.h for details
        struct SetScissor {
            CommandType type = CommandType::eSetScissor;
            UInt32 offset;
            Rect scissor;
        } setScissor;

        /// See CommandType.h for details
        struct BindPacked {
            CommandType type = CommandType::eBindPacked;
            BindID bindID;
            USize constantOffset = 0;
            USize size;
        } bindPacked;

        /// See CommandType.h for details
        struct BindDynamic {
            CommandType type = CommandType::eBindDynamic;
            BindID bindID;
            USize constantOffset = 0;
            USize size;
        } bindDynamic;

        /// See CommandType.h for details
        struct SetSampleMode {
            CommandType type = CommandType::eSetSampleMode;
            GHA::SampleMode sampleMode;
        } setSampleMode;

        /// See CommandType.h for details
        struct BindShader {
            CommandType type = CommandType::eBindShader;
            IShader* shader;
        } bindShader;

        /// See CommandType.h for details
        struct BindShaderBlob {
            CommandType type = CommandType::eBindShaderBlob;
            ShaderBlob* blob;
        } bindShaderBlob;

        /// See CommandType.h for details
        struct Draw {
            CommandType type = CommandType::eDraw;
            UInt32 vertexCount;
            UInt32 instanceCount;
            UInt32 vertexOffset;
            UInt32 instanceOffset;
        } draw;

        /// See CommandType.h for details
        struct DrawIndexed {
            CommandType type = CommandType::eDrawIndexed;
            UInt32 indexCount;
            UInt32 instanceCount;
            UInt32 vertexOffset;
            UInt32 indexOffset;
            UInt32 instanceOffset;
        } drawIndexed;

        /// See CommandType.h for details
        struct DrawIndirect {
            CommandType type = CommandType::eDrawIndirect;
            ResourceID resource;
            UInt32 count;
            UInt32 offset;
            UInt32 stride;
        } drawIndirect;

        /// See CommandType.h for details
        struct DrawIndirectIndexed {
            CommandType type = CommandType::eDrawIndirectIndexed;
            ResourceID resource;
            UInt32 count;
            UInt32 offset;
            UInt32 stride;
        } drawIndirectIndexed;

        /// See CommandType.h for details
        struct Dispatch {
            CommandType type = CommandType::eDispatch;
            UInt32 x;
            UInt32 y;
            UInt32 z;
        } dispatch;

        /// See CommandType.h for details
        struct DispatchIndirect {
            CommandType type = CommandType::eDispatchIndirect;
            ResourceID resource;
        } dispatchIndirect;

        /// See CommandType.h for details
        struct SignalFence {
            CommandType type = CommandType::eSignalFence;
            FenceID fence;
        } signalFence;

        /// See CommandType.h for details
        struct WaitFence {
            CommandType type = CommandType::eWaitFence;
            FenceID fence;
        } waitFence;

        // Type wrapper
        CommandType type;
    };

    // Command should not exceed 164 bytes
    // static_assert(sizeof(Command) <= 164, "GHA command union has an unexpected size");
}