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

#include <COM/IUnknown.h>
#include <vulkan/vulkan.ext.hpp>
#include <GHA/Rect.h>
#include <GHA/Viewport.h>
#include <GHA/PipelineScheduler/CommandListDesc.h>
#include <GHA/PipelineScheduler/TextureBarrierDesc.h>
#include <GHA/BindPoint.h>
#include <GHA/MemoryBounds.h>
#include <GHA/Stage.h>
#include <GHA/ClearValue.h>

namespace Djinn::GHA {
    class IVertexBuffer;
    class IIndexBuffer;
    class IResource;
    class IMemoryResource;
}

namespace Djinn::GHA::Vulkan {
    class Pipeline;
    class DescriptorSet;
    class PipelineLayout;
    class Pass;
    class Frame;
    class TextureView;

    /// Texture barrier desc
    struct TextureBarrierDesc {
        TextureBarrierDesc& TextureView(Vulkan::TextureView* value) {
            textureView = value;
            return *this;
        }

        TextureBarrierDesc& Layouts(PipelineScheduler::LayoutRange value) {
            layouts = value;
            return *this;
        }

        TextureBarrierDesc& MipBounds(MipBounds value) {
            mipBounds = value;
            return *this;
        }

        // Target texture view
        Vulkan::TextureView* textureView;

        // Layout range
        PipelineScheduler::LayoutRange layouts;

        // Mip bounds
        Core::Optional<GHA::MipBounds> mipBounds;
    };

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN CommandList : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        CommandList(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize
        /// \return
        COM::Result Initialize(const PipelineScheduler::CommandListDesc &desc, vk::Device device, vk::CommandPool &pool);

        /// Commands
        void Begin();
        void End();
        void Reset();
        void SetTextureLayouts(UInt32 count, TextureBarrierDesc *descs);
        void SetVertexBuffers(UInt32 count, IVertexBuffer **buffer, USize *bufferOffsets, UInt32 offset);
        void SetIndexBuffer(GHA::IIndexBuffer *buffer, UInt32 offset);
        void SetPipeline(Pipeline *pipeline);
        void SetViewports(GHA::Viewport *viewports, UInt32 count);
        void SetScissors(GHA::Rect *viewports, UInt32 count);
        void SetDescriptorSets(GHA::BindPoint binding, PipelineLayout *layout, UInt32 offset, UInt32 count, DescriptorSet **sets);
        void CopyResource(GHA::IResource *src, TextureLayout srcLayout, IResource *dst, TextureLayout dstLayout);
        void UpdateResource(GHA::IMemoryResource *resource, const MemoryBounds &bounds, void *data);
        void SplatResource(GHA::IMemoryResource *Resource, const MemoryBounds &bounds, UInt32 value);
        void UpdateConstant(PipelineLayout *layout, const StageSet &stages, const MemoryBounds &bounds, void *data);
        void BeginPass(Pass *pass, Frame *frame, const Rect &rect, UInt32 colorClearCount, ColorClearValue *colorClears, StencilClearValue *stencilClear);
        void EndPass();
        void Draw(UInt32 vertexCount, UInt32 vertexOffset);
        void DrawIndexed(UInt32 indexCount, UInt32 instanceCount, UInt32 indexOffset, UInt32 vertexOffset, UInt32 instanceOffset);
        void Draw(UInt32 vertexCount, UInt32 instanceCount, UInt32 vertexOffset, UInt32 instanceOffset);
        void ClearColor(TextureView *view, const ColorClearValue &clear);
        void ClearDepth(TextureView *view, float depth);
        void ClearStencil(TextureView *view, UInt32 stencil);
        void ClearDepthStencil(TextureView *view, float depth, UInt32 stencil);
        const PipelineScheduler::CommandListDesc &GetDesc();

        /// Get command buffer
        /// \return
        const vk::CommandBuffer &GetBuffer() const {
            return buffer;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        PipelineScheduler::CommandListDesc desc;
        vk::CommandPool pool;
        vk::Device device;
        vk::CommandBuffer buffer;
    };
}