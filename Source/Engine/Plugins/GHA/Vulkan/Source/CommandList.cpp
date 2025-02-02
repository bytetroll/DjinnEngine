#include <GHA/Vulkan/CommandList.h>
#include <GHA/Vulkan/Texture.h>
#include <GHA/Vulkan/TextureView.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <GHA/Vulkan/VertexBuffer.h>
#include <GHA/Vulkan/ConstantBuffer.h>
#include <GHA/Vulkan/IndexBuffer.h>
#include <GHA/Vulkan/VertexBuffer.h>
#include <GHA/Vulkan/EnumUtils.h>
#include <GHA/Vulkan/Pipeline.h>
#include <GHA/Vulkan/DescriptorSet.h>
#include <GHA/Vulkan/DescriptorSetLayout.h>
#include <GHA/Viewport.h>
#include <GHA/Vulkan/PipelineLayout.h>
#include <GHA/Vulkan/Pass.h>
#include <Core/Memory.h>
#include <GHA/Vulkan/Frame.h>

DJINN_NS3(GHA, Vulkan);

CommandList::CommandList(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUnknown(registry, outer, host) {

}

COM::Result CommandList::Initialize(const PipelineScheduler::CommandListDesc &desc, vk::Device device, vk::CommandPool &pool) {
    this->desc = desc;
    this->device = device;
    this->pool = pool;

    // Attempt to allocate command buffer
    vk::CommandBufferAllocateInfo allocateInfo;
    allocateInfo.commandPool = pool;
    allocateInfo.commandBufferCount = 1;
    allocateInfo.level = vk::CommandBufferLevel::ePrimary;
    if (vk::CheckedResult result = device.allocateCommandBuffers(&allocateInfo, &buffer)) {
        ErrorLog(this).Write("Failed to allocate vulkan command buffer: " , vk::to_string(result).c_str());
        return COM::kError;
    }

    // ...
    return COM::kOK;
}

void Vulkan::CommandList::Begin() {
    vk::CommandBufferBeginInfo info;
    if (vk::CheckedResult result = buffer.begin(&info)) {
        ErrorLog(this).Write("Failed to end vulkan command buffer: ", vk::to_string(result).c_str());
    }
}

void Vulkan::CommandList::End() {
    if (vk::CheckedResult result = buffer.end()) {
        ErrorLog(this).Write("Failed to end vulkan command buffer: ", vk::to_string(result).c_str());
    }
}

void Vulkan::CommandList::Reset() {
    if (vk::CheckedResult result = buffer.reset({})) {
        ErrorLog(this).Write("Failed to end vulkan command buffer: ", vk::to_string(result).c_str());
    }
}

void CommandList::SetTextureLayouts(UInt32 count, TextureBarrierDesc *descs) {
    Int64 remaining = count;
    while (remaining > 0) {
        // Batched submit count
        auto min = static_cast<UInt32>(Math::Min(remaining, 16));

        // Collect barriers
        vk::ImageMemoryBarrier barriers[16];
        for (Int64 i = 0; i < min; i++) {
            auto textureDesc = descs[i].textureView->GetDesc();

            // ...
            barriers[i].oldLayout = AsImageLayout(descs[i].layouts.lower);
            barriers[i].newLayout = AsImageLayout(descs[i].layouts.upper);
            barriers[i].image = static_cast<Texture *>(static_cast<TextureView *>(descs[i].textureView)->GetTexture())->GetImage();
            barriers[i].subresourceRange = vk::ImageSubresourceRange(
                    AsImageAspectFlag(textureDesc.viewType),
                    static_cast<UInt32> (descs[i].mipBounds.IsValid() ? descs[i].mipBounds->lower : textureDesc.mipBounds->lower),
                    static_cast<UInt32> (descs[i].mipBounds.IsValid() ? (descs[i].mipBounds->upper - descs[i].mipBounds->lower) : (textureDesc.mipBounds->upper - textureDesc.mipBounds->lower)),
                    textureDesc.arrayBounds->lower,
                    textureDesc.arrayBounds->upper - textureDesc.arrayBounds->lower
            );

            barriers[i].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barriers[i].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

            // Previous layout
            switch (descs[i].layouts.lower) {
                default:
                    break;
                case GHA::TextureLayout::eUndefined:
                    barriers[i].srcAccessMask = static_cast<vk::AccessFlagBits>(0);
                    break;
                case GHA::TextureLayout::eColorRenderTarget:
                    barriers[i].srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
                    break;
                case GHA::TextureLayout::eDepthStencilRenderTarget:
                    barriers[i].srcAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentWrite;
                    break;
                case GHA::TextureLayout::eDepthStencilRead:
                    barriers[i].srcAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead;
                    break;
                case GHA::TextureLayout::eTransferSrc:
                    barriers[i].srcAccessMask = vk::AccessFlagBits::eTransferRead;
                    break;
                case GHA::TextureLayout::eTransferDst:
                    barriers[i].srcAccessMask = vk::AccessFlagBits::eTransferWrite;
                    break;
                case GHA::TextureLayout::eColorRead:
                    barriers[i].srcAccessMask = vk::AccessFlagBits::eShaderRead;
                    break;
                case GHA::TextureLayout::ePresentSrc:
                    barriers[i].srcAccessMask = vk::AccessFlagBits::eMemoryRead;
                    break;
            }

            // New layout
            switch (descs[i].layouts.upper) {
                default:
                    break;
                case GHA::TextureLayout::ePresentSrc:
                    barriers[i].dstAccessMask = vk::AccessFlagBits::eMemoryRead;
                    break;
                case GHA::TextureLayout::eColorRenderTarget:
                    if (descs[i].layouts.lower != GHA::TextureLayout::eUndefined) {
                        // TODO: Check, Barrier.srcAccessMask |= vk::AccessFlagBits::eTransferRead;
                    }
                    barriers[i].dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
                    break;
                case GHA::TextureLayout::eTransferSrc:
                    if (descs[i].layouts.lower != GHA::TextureLayout::eUndefined) {
                        // TODO: Check, Barrier.srcAccessMask |= vk::AccessFlagBits::eTransferRead;
                    }
                    barriers[i].dstAccessMask = vk::AccessFlagBits::eTransferRead;
                    break;
                case GHA::TextureLayout::eDepthStencilRenderTarget:
                    barriers[i].dstAccessMask |= vk::AccessFlagBits::eDepthStencilAttachmentWrite;
                    break;
                case GHA::TextureLayout::eColorRead:
                    if (barriers[i].srcAccessMask == static_cast<vk::AccessFlagBits>(0) && descs[i].layouts.lower != GHA::TextureLayout::eUndefined) {
                        barriers[i].srcAccessMask = ( vk::AccessFlagBits::eHostWrite |  vk::AccessFlagBits::eTransferWrite);
                    }
                    barriers[i].dstAccessMask = vk::AccessFlagBits::eShaderRead;
                    break;
                case GHA::TextureLayout::eTransferDst:
                    barriers[i].dstAccessMask = vk::AccessFlagBits::eTransferWrite;
                    break;
                case GHA::TextureLayout::eDepthStencilRead:
                    barriers[i].dstAccessMask = ( vk::AccessFlagBits::eShaderRead |  vk::AccessFlagBits::eDepthStencilAttachmentRead);
                    break;
            }
        }

        // Submit
        buffer.pipelineBarrier(
                vk::PipelineStageFlagBits::eAllCommands, vk::PipelineStageFlagBits::eAllCommands, {},
                0, nullptr,
                0, nullptr,
                min, barriers
        );

        /// ...
        remaining -= 16;
        descs += 16;
    }
}

void Vulkan::CommandList::SetVertexBuffers(UInt32 count, GHA::IVertexBuffer **buffers, USize *bufferOffsets, UInt32 offset) {
    vk::Buffer vkBuffers[16];
    for (UInt32 i = 0; i < count; i++) {
        vkBuffers[i] = static_cast<VertexBuffer *>(buffers[i])->GetBuffer();
    }
    vk::DeviceSize vkSize[16];
    for (UInt32 i = 0; i < count; i++) {
        vkSize[i] = bufferOffsets[i];
    }
    buffer.bindVertexBuffers(offset, count, vkBuffers, vkSize);
}

void Vulkan::CommandList::SetIndexBuffer(GHA::IIndexBuffer *_buffer, UInt32 offset) {
    buffer.bindIndexBuffer(static_cast<IndexBuffer *>( _buffer)->GetBuffer(), offset, AsIndexType(_buffer->GetDesc().type));
}

void Vulkan::CommandList::SetPipeline(Pipeline *pipeline) {
    buffer.bindPipeline(AsBindPoint(pipeline->GetBindPoint()), pipeline->GetPipeline());
}

void Vulkan::CommandList::SetViewports(GHA::Viewport *_viewports, UInt32 count) {
    vk::Viewport viewports[32];
    for (UInt32 i = 0; i < count; i++) {
        viewports[i].x = _viewports[i].rect.offset.width;
        viewports[i].y = _viewports[i].rect.offset.height;
        viewports[i].width = _viewports[i].rect.extent.width;
        viewports[i].height = _viewports[i].rect.extent.height;
        viewports[i].minDepth = _viewports[i].minDepth;
        viewports[i].maxDepth = _viewports[i].maxDepth;
    }
    buffer.setViewport(0, count, viewports);
}

void Vulkan::CommandList::SetScissors(GHA::Rect *_scissors, UInt32 count) {
    vk::Rect2D scissors[32];
    for (UInt32 i = 0; i < count; i++) {
        scissors[i].offset.x = _scissors[i].offset.width;
        scissors[i].offset.y = _scissors[i].offset.height;
        scissors[i].extent.width = _scissors[i].extent.width;
        scissors[i].extent.height = _scissors[i].extent.height;
    }
    buffer.setScissor(0, count, scissors);
}

void Vulkan::CommandList::SetDescriptorSets(GHA::BindPoint binding, PipelineLayout *layout, UInt32 offset, UInt32 count, DescriptorSet **sets) {
    // Collect sets
    vk::DescriptorSet vkSets[32];
    for (UInt32 i = 0; i < count; i++) {
        vkSets[i] = sets[i]->GetSet();
    }
    buffer.bindDescriptorSets(AsBindPoint(binding), layout->GetLayout(), offset, count, vkSets, 0, nullptr);
}

void Vulkan::CommandList::CopyResource(GHA::IResource *src, GHA::TextureLayout srcLayout, GHA::IResource *dst, GHA::TextureLayout dstLayout) {
    switch (src->GetClassID()) {
        case TextureView::kCID: {
            switch (src->GetClassID()) {
                case TextureView::kCID: {
                    auto srcViewDesc = static_cast<TextureView*>(src)->GetDesc();
                    auto dstViewDesc = static_cast<TextureView*>(dst)->GetDesc();
                    auto srcDesc = static_cast<TextureView*>(src)->GetTexture()->GetDesc();
                    auto dstDesc = static_cast<TextureView*>(dst)->GetTexture()->GetDesc();
                    vk::ImageBlit blit;
                    blit.srcOffsets[0] = blit.dstOffsets[0] = vk::Offset3D(0, 0, 0);
                    blit.srcOffsets[1] = vk::Offset3D(srcDesc.size.width, srcDesc.size.height, srcDesc.size.depth);
                    blit.dstOffsets[1] = vk::Offset3D(dstDesc.size.width, dstDesc.size.height, dstDesc.size.depth);
                    blit.srcSubresource = vk::ImageSubresourceLayers(
                            AsImageAspectFlag(srcViewDesc.viewType),
                            srcViewDesc.mipBounds->lower,
                            srcViewDesc.arrayBounds->lower,
                            srcViewDesc.arrayBounds->upper - srcViewDesc.arrayBounds->lower
                    );
                    blit.dstSubresource = vk::ImageSubresourceLayers(
                            AsImageAspectFlag(dstViewDesc.viewType),
                            dstViewDesc.mipBounds->lower,
                            dstViewDesc.arrayBounds->lower,
                            dstViewDesc.arrayBounds->upper - dstViewDesc.arrayBounds->lower
                    );
                    buffer.blitImage(
                            static_cast<Texture *>(static_cast<TextureView*>(src)->GetTexture())->GetImage(), AsImageLayout(srcLayout),
                            static_cast<Texture *>(static_cast<TextureView*>(dst)->GetTexture())->GetImage(), AsImageLayout(dstLayout),
                            1, &blit, vk::Filter::eLinear
                    );
                    return;
                }
            }
            break;
        }
    }

    // ...
    ErrorLog(this).Write("Unknown memory resource copy operation");
}

void Vulkan::CommandList::UpdateResource(GHA::IMemoryResource *resource, const GHA::MemoryBounds &bounds, void *data) {
    switch (resource->GetClassID()) {
        case VertexBuffer::kCID:
            buffer.updateBuffer(static_cast<VertexBuffer*>(resource)->GetBuffer(), bounds.beginOffset.width, bounds.endOffset.width - bounds.beginOffset.width, data);
            break;
        case IndexBuffer::kCID:
            buffer.updateBuffer(static_cast<IndexBuffer*>(resource)->GetBuffer(), bounds.beginOffset.width, bounds.endOffset.width - bounds.beginOffset.width, data);
            break;
        case ConstantBuffer::kCID:
            buffer.updateBuffer(static_cast<ConstantBuffer*>(resource)->GetBuffer(), bounds.beginOffset.width, bounds.endOffset.width - bounds.beginOffset.width, data);
            break;
    }
}

void Vulkan::CommandList::SplatResource(GHA::IMemoryResource *Resource, const GHA::MemoryBounds &bounds, UInt32 value) {
}

void Vulkan::CommandList::UpdateConstant(PipelineLayout *layout, const GHA::StageSet &stages, const GHA::MemoryBounds &bounds, void *data) {
    buffer.pushConstants(layout->GetLayout(), AsShaderStageFlags(stages), bounds.beginOffset.width, (bounds.endOffset.width - bounds.beginOffset.width),data);
}

void CommandList::BeginPass(Pass *pass, Frame *frame, const GHA::Rect &rect, UInt32 colorClearCount, GHA::ColorClearValue *colorClears, GHA::StencilClearValue *stencilClear) {
    // Collect values
    vk::ClearValue clearValues[16];
    for (UInt32 i = 0; i < colorClearCount; i++) {
        vk::ClearColorValue value;
        Core::Memory::Copy(&value, &colorClears[i], sizeof(vk::ClearColorValue));
        clearValues[i] = value;
    }
    if (stencilClear) {
        vk::ClearDepthStencilValue value;
        value.stencil = stencilClear->stencil;
        value.depth = stencilClear->depth;
        clearValues[colorClearCount] = value;
    }

    // Begin pass
    vk::RenderPassBeginInfo beginInfo;
    beginInfo.renderArea = vk::Rect2D(vk::Offset2D(rect.offset.width, rect.offset.height), vk::Extent2D(rect.extent.width, rect.extent.height));
    beginInfo.renderPass = (pass)->GetPass();
    beginInfo.framebuffer = (frame)->GetFrameBuffer();
    beginInfo.clearValueCount = colorClearCount + (stencilClear != nullptr);
    beginInfo.pClearValues = clearValues;
    buffer.beginRenderPass(&beginInfo, vk::SubpassContents::eInline);
}

void Vulkan::CommandList::EndPass() {
    buffer.endRenderPass();
}

void Vulkan::CommandList::Draw(UInt32 vertexCount, UInt32 vertexOffset) {
    buffer.draw(vertexCount, 1, vertexOffset, 0);
}

void Vulkan::CommandList::DrawIndexed(UInt32 indexCount, UInt32 instanceCount, UInt32 indexOffset, UInt32 vertexOffset, UInt32 instanceOffset) {
    buffer.drawIndexed(indexCount, instanceCount, indexOffset, vertexOffset, instanceOffset);
}

void Vulkan::CommandList::Draw(UInt32 vertexCount, UInt32 instanceCount, UInt32 vertexOffset, UInt32 instanceOffset) {
    buffer.draw(vertexCount, instanceCount, vertexOffset, instanceOffset);
}

void Vulkan::CommandList::ClearColor(TextureView *view, const GHA::ColorClearValue &clear) {
    vk::ClearColorValue clearColorValue;
    Core::Memory::Copy(&clearColorValue, &clear, sizeof(vk::ClearColorValue));

    auto desc = view->GetDesc();

    // ...
    vk::ImageSubresourceRange range(
            AsImageAspectFlag(view->GetDesc().viewType),
            desc.mipBounds->lower,
            desc.mipBounds->upper - desc.mipBounds->lower,
            desc.arrayBounds->lower,
            desc.arrayBounds->upper - desc.arrayBounds->lower
    );

    // ...
    buffer.clearColorImage(static_cast<Texture *>(view->GetTexture())->GetImage(), vk::ImageLayout::eTransferDstOptimal, &clearColorValue, 1, &range);
}

void Vulkan::CommandList::ClearDepth(TextureView *view, float depth) {
    vk::ImageSubresourceRange range(
            vk::ImageAspectFlagBits::eDepth,
            0, 1, 0, 1
    );
    vk::ClearDepthStencilValue clearValue;
    clearValue.depth = depth;
    buffer.clearDepthStencilImage(static_cast<Texture *>(view->GetTexture())->GetImage(), vk::ImageLayout::eTransferDstOptimal, &clearValue, 1, &range);
}

void Vulkan::CommandList::ClearStencil(TextureView *view, UInt32 stencil) {
    vk::ImageSubresourceRange range(
            vk::ImageAspectFlagBits::eStencil,
            0, 1, 0, 1
    );
    vk::ClearDepthStencilValue clearValue;
    clearValue.stencil = stencil;
    buffer.clearDepthStencilImage(static_cast<Texture *>(view->GetTexture())->GetImage(), vk::ImageLayout::eTransferDstOptimal, &clearValue, 1, &range);
}

void Vulkan::CommandList::ClearDepthStencil(TextureView *view, float depth, UInt32 stencil) {
    vk::ImageSubresourceRange range(
            vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil,
            0, 1, 0, 1
    );
    vk::ClearDepthStencilValue clearValue;
    clearValue.stencil = stencil;
    clearValue.depth = depth;
    buffer.clearDepthStencilImage(static_cast<Texture *>(view->GetTexture())->GetImage(), vk::ImageLayout::eTransferDstOptimal, &clearValue, 1, &range);
}

const PipelineScheduler::CommandListDesc &Vulkan::CommandList::GetDesc() {
    return desc;
}

COM::Result CommandList::Released() {
    device.freeCommandBuffers(pool, 1, &buffer);

    // OK
    return COM::kOK;
}
