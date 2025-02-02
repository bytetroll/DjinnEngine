//
// Created by Dev on 12/26/2017.
//
#pragma once

#include <Core/Pool.h>
#include <GHA/MultiSampleStateDesc.h>
#include <Core/HashMap.h>
#include <Data/Asset.h>
#include <GHA/SchedulerContext.h>
#include "DependencyMode.h"
#include "PassNode.h"
#include "ID.h"
#include "VertexArray.h"
#include "IndexArray.h"

namespace Djinn::Data {
    class ITextureAsset;
    class IMeshAsset;
    class ShaderAsset;
}

namespace Djinn::Graphics {
    struct PassContext {
        void Clear();

        struct TextureBind {
            GHA::TextureID id;
            GHA::TextureViewDesc desc;
            GHA::SamplerDesc sampler;
        };

        Core::HashMap<GHA::BindID, TextureBind> mappedTextures;
    };

    class DJINN_EXPORT_PLUGINS_GRAPHICS_GRAPH PassBuilder {
    public:
        ~PassBuilder();

        /// Make a builder
        /// \param name
        /// \param pool
        /// \param schedulerContext
        /// \return
        static PassBuilder Make(const char *name, Core::LazyObjectPool<PassNode> *pool, PassContext* passContext, GHA::SchedulerContext *schedulerContext);

        /// Create a pass
        /// \return
        PassBuilder Pass(const char *name);

        /// Create an inlined pass
        /// \return
        PassBuilder InlinedPass(const char *name);

        /// Add a dependency
        /// \param other
        PassBuilder &AddDependency(const PassBuilder &other);

        /// Push debug event
        /// \param name - Name of this event
        void PushDebugEvent(const char *name) {
            GHA::Command command;
            command.debugPushEvent = {};
            command.debugPushEvent.name = name;
            passNode->node->commands.Add(command);
        }

        /// Pop debug event
        void PopDebugEvent() {
            GHA::Command command;
            command.debugPopEvent = {};
            passNode->node->commands.Add(command);
        }

        /// Set primitive topology
        /// \param topology
        void SetTopology(GHA::Topology topology) {
            GHA::Command command;
            command.setTopology = {};
            command.setTopology.topology = topology;
            passNode->node->commands.Add(command);
        }

        /// Create dynamic texture
        /// \param desc - Description of texture
        /// \return - Resource wrapper
        Texture AddTexture(const GHA::TextureDesc &desc) {
            GHA::ResourceCommand command;
            command.addTexture = {};
            command.addTexture.desc = desc;
            command.addTexture.resource = {schedulerContext->CreateUID()};
            schedulerContext->resources.Add({passNode->node, command});
            return Texture(command.addTexture.resource, desc);
        }

        /// Import an external texture
        /// \param texture - Texture handle to be imported
        /// \return - Resource wrapper
        Texture AddTexture(GHA::ITexture *texture);

        /// Import a texture asset
        /// \param asset
        /// \return
        Texture AddTexture(const Data::AssetView<Data::ITextureAsset>& asset);

        /// Import a texture asset
        /// \param asset
        /// \return
        Mesh AddMesh(const Data::AssetView<Data::IMeshAsset>& asset);

        /// Import an external vertex buffer
        /// \param buffer - Buffer handle to be imported
        /// \return - Resource wrapper
        VertexBuffer AddVertexBuffer(GHA::IVertexBuffer *buffer);

        /// Import an external vertex buffer
        /// \param buffer - Buffer handle to be imported
        /// \return - Resource wrapper
        template<typename T>
        VertexBuffer AddVertexArray(VertexArray<T> &vertices, bool highFrequency) {
            return AddVertexBuffer(static_cast<GHA::IStreamable *>(&vertices), GHA::VertexBufferDesc().Make<T>(0), highFrequency);
        }

        /// Import an external vertex buffer
        /// \param buffer - Buffer handle to be imported
        /// \return - Resource wrapper
        VertexBuffer AddVertexBuffer(GHA::IStreamable *stream, const GHA::VertexBufferDesc &desc, bool highFrequency = false);

        /// Import an external index buffer
        /// \param buffer - Buffer handle to be imported
        /// \return - Resource wrapper
        IndexBuffer AddIndexBuffer(GHA::IIndexBuffer *buffer);

        /// Import an external index buffer
        /// \param buffer - Buffer handle to be imported
        /// \return - Resource wrapper
        template<typename T>
        IndexBuffer AddIndexArray(IndexArray<T> &indices, bool highFrequency) {
            return AddIndexBuffer(static_cast<GHA::IStreamable *>(&indices), GHA::IndexBufferDesc().Make<T>(0), highFrequency);
        }

        /// Import an external index buffer
        /// \param buffer - Buffer handle to be imported
        /// \return - Resource wrapper
        IndexBuffer AddIndexBuffer(GHA::IStreamable *stream, const GHA::IndexBufferDesc &desc, bool highFrequency = false);

        /// Copy contents of one resource to another
        /// \param source - Source resource
        /// \param dest - Destination resource
        void CopyTexture(const Texture &source, const Texture &dest) {
            GHA::Command command;
            command.cmdCopyTexture = {};
            command.cmdCopyTexture.source = source;
            command.cmdCopyTexture.dest = dest;
            passNode->node->commands.Add(command);
        }

        /// Clear a resource with the optimal values
        /// \param resource - Resource to  be cleared
        void Clear(const GHA::ResourceID &resource) {
            GHA::Command command;
            command.cmdClear = {};
            command.cmdClear.resource = resource;
            passNode->node->commands.Add(command);
        }

        /// Clear resource and fill with color value
        /// \param texture - Texture to  be cleared
        void ClearColor(const Texture &texture) {
            GHA::Command command;
            command.cmdClearColor = {};
            command.cmdClearColor.resource = texture;
            passNode->node->commands.Add(command);
        }

        /// Clear resource and fill with depth & stencil value
        /// \param texture - Texture to  be cleared
        void ClearDepthStencil(const Texture &texture) {
            GHA::Command command;
            command.cmdClearDepthStencil = {};
            command.cmdClearDepthStencil.resource = texture;
            passNode->node->commands.Add(command);
        }

        /// Bind a single render target
        /// \param bind - ID of bind, Fx. "target0"_id
        /// \param texture - Texture to be bound
        void BindRenderTarget(UInt32 index, const Texture &texture, const GHA::TextureViewDesc &viewDesc, const GHA::RenderTargetBlendDesc &blendDesc) {
            GHA::Command command;
            command.bindRenderTarget = {};
            command.bindRenderTarget.offset = index;
            command.bindRenderTarget.texture = texture;
            command.bindRenderTarget.viewDesc = viewDesc;
            command.bindRenderTarget.blendDesc = blendDesc;
            passNode->node->commands.Add(command);
        }

        /// Bind the depth render target
        /// \param bind - ID of bind, Fx. "target0"_id
        /// \param texture - Texture to be bound
        void BindDepthRenderTarget(const Texture &texture, const GHA::TextureViewDesc &viewDesc) {
            GHA::Command command;
            command.bindDepthRenderTarget = {};
            command.bindDepthRenderTarget.texture = texture;
            command.bindDepthRenderTarget.viewDesc = viewDesc;
            passNode->node->commands.Add(command);
        }

        /// Bind a single resource
        /// \param bind - ID of bind, Fx. "albedo"_id
        /// \param resource - Resource to be bound
        void BindResource(const GHA::BindID &bind, const GHA::ResourceID &resource) {
            GHA::Command command;
            command.bindResource = {};
            command.bindResource.bindID = bind;
            command.bindResource.resource = resource;
            passNode->node->commands.Add(command);
        }

        /// Bind a single texture
        /// \param bind - ID of bind, Fx. "albedo"_id
        /// \param resource - Resource to be bound
        void BindTexture(const GHA::BindID &bind, const GHA::TextureID &resource, const GHA::TextureViewDesc &viewDesc, const GHA::SamplerDesc &samplerDesc) {
            GHA::Command command;
            command.bindTexture = {};
            command.bindTexture.bindID = bind;
            command.bindTexture.texture = resource;
            command.bindTexture.viewDesc = viewDesc;
            command.bindTexture.samplerDesc = samplerDesc;
            command.bindTexture.arrayIndex = -1;
            passNode->node->commands.Add(command);
        }

        /// Bind a single texture
        /// \param bind - ID of bind, Fx. "albedo"_id
        /// \param resource - Resource to be bound
        void BindTexture(const GHA::BindID &bind, UInt16 index, const GHA::TextureID &resource, const GHA::TextureViewDesc &viewDesc, const GHA::SamplerDesc &samplerDesc) {
            GHA::Command command;
            command.bindTexture = {};
            command.bindTexture.bindID = bind;
            command.bindTexture.texture = resource;
            command.bindTexture.viewDesc = viewDesc;
            command.bindTexture.samplerDesc = samplerDesc;
            command.bindTexture.arrayIndex = index;
            passNode->node->commands.Add(command);
        }

        /// Bind packed parameters
        /// \param bind
        /// \param packed
        /// \param size
        void BindPacked(const GHA::BindID &bind, void *packed, USize size) {
            GHA::Command command;
            command.bindPacked = {};
            command.bindPacked.bindID = bind;
            command.bindPacked.size = size;
            command.bindPacked.constantOffset = schedulerContext->GetConstantOffset(packed, size);
            passNode->node->commands.Add(command);
        }

        /// Bind packed parameters
        /// \param bind
        /// \param packed
        template<typename T>
        void BindPacked(const GHA::BindID &bind, T *packed) {
            BindPacked(bind, packed, sizeof(T));
        }

        /// Bind dynamic parameters
        /// \param bind
        /// \param packed
        /// \param size
        void BindDynamic(const GHA::BindID &bind, void *dynamic, USize size) {
            GHA::Command command;
            command.bindDynamic = {};
            command.bindDynamic.bindID = bind;
            command.bindDynamic.size = size;
            command.bindDynamic.constantOffset = schedulerContext->GetConstantOffset(dynamic, size);
            passNode->node->commands.Add(command);
        }

        /// Bind dynamic parameters
        /// \param bind
        /// \param packed
        template<typename T>
        void BindDynamic(const GHA::BindID &bind, T *dynamic) {
            BindDynamic(bind, dynamic, sizeof(T));
        }

        /// Bind a single vertex buffer
        /// \param bind - ID of bind, Fx "base"_id
        /// \param vertexBuffer - Vertex buffer to be bound
        void BindVertexBuffer(UInt32 index, const VertexBuffer &vertexBuffer) {
            GHA::Command command;
            command.bindVertexBuffer = {};
            command.bindVertexBuffer.offset = index;
            command.bindVertexBuffer.vertexBuffer = vertexBuffer;
            passNode->node->commands.Add(command);
        }

        /// Bind index buffer
        /// \param indexBuffer - Index buffer to be bound
        void BindIndexBuffer(const IndexBuffer &indexBuffer) {
            GHA::Command command;
            command.bindIndexBuffer = {};
            command.bindIndexBuffer.buffer = indexBuffer;
            passNode->node->commands.Add(command);
        }

        /// Set the rasterizer state to be used
        /// \param desc - Description
        void SetRasterizerState(const GHA::RasterizerStateDesc &desc) {
            GHA::Command command;
            command.setRasterizerState = {};
            command.setRasterizerState.desc = desc;
            passNode->node->commands.Add(command);
        }

        /// Set sample mode
        /// \param mode
        void SetSampleMode(GHA::SampleMode mode) {
            GHA::Command command;
            command.setSampleMode = {};
            command.setSampleMode.sampleMode = mode;
            passNode->node->commands.Add(command);
        }

        /// Set the depth stencil state to be used
        /// \param desc - Description
        void SetDepthStencilState(const GHA::DepthStencilStateDesc &desc) {
            GHA::Command command;
            command.setDepthStencilState = {};
            command.setDepthStencilState.desc = desc;
            passNode->node->commands.Add(command);
        }

        /// Set a single viewport
        /// \param bind - ID of bind, Fx "target0"_id
        /// \param desc - Viewport to be bound
        void SetViewport(const GHA::Viewport &desc) {
            GHA::Command command;
            command.setViewport = {};
            command.setViewport.offset = 0;
            command.setViewport.viewport = desc;
            passNode->node->commands.Add(command);
        }

        /// Set a single scissor
        /// \param bind - ID of bind, Fx "target0"_id
        /// \param desc - Scissor to be bound
        void SetScissor(const GHA::Rect &desc) {
            GHA::Command command;
            command.setScissor = {};
            command.setScissor.offset = 0;
            command.setScissor.scissor = desc;
            passNode->node->commands.Add(command);
        }

        /// Bind a single shader
        /// \param shader - Shader to be bound
        void BindShader(GHA::IShader *shader);

        /// Bind a single shader
        /// \param shader - Shader to be bound
        void BindShader(const Data::AssetView<Data::ShaderAsset>& asset);

        /// Draw non indexed
        /// \param vertexCount - Number of vertices
        /// \param instanceCount - Number of instances
        /// \param vertexOffset - First vertex
        /// \param instanceOffset - First instance
        void Draw(UInt32 vertexCount, UInt32 instanceCount = 1, UInt32 vertexOffset = 0, UInt32 instanceOffset = 0) {
            GHA::Command command;
            command.draw = {};
            command.draw.vertexCount = vertexCount;
            command.draw.instanceCount = instanceCount;
            command.draw.vertexOffset = vertexOffset;
            command.draw.instanceOffset = instanceOffset;
            passNode->node->commands.Add(command);
        }

        /// Draw indexed
        /// \param indexCount - Number of indices
        /// \param instanceCount - Number of instances
        /// \param vertexOffset - First vertex
        /// \param indexOffset - First index
        /// \param instanceOffset - First instance
        void DrawIndexed(UInt32 indexCount, UInt32 instanceCount = 1, UInt32 vertexOffset = 0, UInt32 indexOffset = 0, UInt32 instanceOffset = 0) {
            GHA::Command command;
            command.drawIndexed = {};
            command.drawIndexed.indexCount = indexCount;
            command.drawIndexed.instanceCount = instanceCount;
            command.drawIndexed.vertexOffset = vertexOffset;
            command.drawIndexed.indexOffset = indexOffset;
            command.drawIndexed.instanceOffset = instanceOffset;
            passNode->node->commands.Add(command);
        }

        /// Draw indirect non indexed
        /// \param resource - Indirect resource
        /// \param count
        /// \param offset
        /// \param stride
        void DrawIndirect(const GHA::ResourceID &resource, UInt32 count, UInt32 offset, UInt32 stride) {
            GHA::Command command;
            command.drawIndirect = {};
            command.drawIndirect.resource = resource;
            command.drawIndirect.count = count;
            command.drawIndirect.offset = offset;
            command.drawIndirect.stride = stride;
            passNode->node->commands.Add(command);
        }

        /// Draw indirect indexed
        /// \param resource
        /// \param count
        /// \param offset
        /// \param stride
        void DrawIndirectIndexed(const GHA::ResourceID &resource, UInt32 count, UInt32 offset, UInt32 stride) {
            GHA::Command command;
            command.drawIndirect = {};
            command.drawIndirect.resource = resource;
            command.drawIndirect.count = count;
            command.drawIndirect.offset = offset;
            command.drawIndirect.stride = stride;
            passNode->node->commands.Add(command);
        }

        /// Dispatch
        /// \param x - Number of X groups
        /// \param y - Number of Y groups
        /// \param z - Number of Z groups
        void Dispatch(UInt32 x, UInt32 y, UInt32 z) {
            GHA::Command command;
            command.dispatch = {};
            command.dispatch.x = x;
            command.dispatch.y = y;
            command.dispatch.z = z;
            passNode->node->commands.Add(command);
        }

        /// Dispatch indirect
        /// \param resource - Indirect resource
        void DispatchIndirect(const GHA::ResourceID &resource) {
            GHA::Command command;
            command.dispatchIndirect = {};
            command.dispatchIndirect.resource = resource;
            passNode->node->commands.Add(command);
        }

        /// Map a shared texture
        /// \param name
        /// \param id
        void MapSharedTexture(GHA::BindID bind, GHA::TextureID id, const GHA::TextureViewDesc& view, const GHA::SamplerDesc& sampler);

        /// Set the dependency mode
        /// \param value
        /// \return
        PassBuilder &DependencyMode(Graphics::DependencyMode value) {
            passNode->dependencyMode = value;
            return *this;
        }

    private:
        void PrepareSync();
        void BindBlobResources(GHA::ShaderBlob* blob);

    private:
        Core::LazyObjectPool<PassNode> *pool;
        PassContext* passContext;
        PassNode *passNode;
        GHA::SchedulerContext *schedulerContext;
    };
}