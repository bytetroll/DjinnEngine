//
// Created by Dev on 11/25/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <Core/Assert.h>
#include <GHA/Format.h>
#include <GHA/RenderTargetOp.h>
#include <GHA/Stage.h>
#include <GHA/SampleMode.h>
#include <GHA/CullMode.h>
#include <GHA/FillMode.h>
#include <GHA/BlendOp.h>
#include <GHA/CompareOp.h>
#include <GHA/BlendFactor.h>
#include <GHA/BlendStateDesc.h>
#include <GHA/InputDesc.h>
#include <GHA/Topology.h>
#include <GHA/SamplerFilterMode.h>
#include <GHA/SamplerMipMapMode.h>
#include <GHA/SamplerAddressMode.h>
#include <GHA/TextureUsage.h>
#include <GHA/TextureType.h>
#include <GHA/TextureViewType.h>
#include <GHA/TextureLayout.h>
#include <GHA/IndexBufferDesc.h>
#include <GHA/BindPoint.h>
#include <GHA/PipelineScheduler/DescriptorType.h>
#include "ConstantBuffer.h"
#include "TextureView.h"
#include "UnorderedTextureView.h"
#include "UnorderedVertexBufferView.h"
#include "UnorderedIndexBufferView.h"
#include "StructuredBufferView.h"
#include "UnorderedStructuredBufferView.h"
#include "TextureBufferView.h"
#include "UnorderedTextureBufferView.h"

namespace Djinn::GHA::Vulkan {
    static vk::DescriptorType AsDescriptorType(PipelineScheduler::DescriptorType value) {
        switch (value) {
            default:
                break;
            case PipelineScheduler::DescriptorType::eConstantBuffer:
                return vk::DescriptorType::eUniformBuffer;
            case PipelineScheduler::DescriptorType::eTextureView:
                return vk::DescriptorType::eCombinedImageSampler;
            case PipelineScheduler::DescriptorType::eUnorderedTextureView:
                return vk::DescriptorType::eStorageImage;
            case PipelineScheduler::DescriptorType::eUnorderedVertexBufferView:
                return vk::DescriptorType::eStorageBuffer;
            case PipelineScheduler::DescriptorType::eUnorderedIndexBufferView:
                return vk::DescriptorType::eStorageTexelBuffer;
            case PipelineScheduler::DescriptorType::eStructuredBufferView:
                return vk::DescriptorType::eUniformBuffer;
            case PipelineScheduler::DescriptorType::eUnorderedStructuredBufferView:
                return vk::DescriptorType::eStorageBuffer;
            case PipelineScheduler::DescriptorType::eTextureBufferView:
                return vk::DescriptorType::eUniformTexelBuffer;
            case PipelineScheduler::DescriptorType::eUnorderedTextureBufferView:
                return vk::DescriptorType::eStorageTexelBuffer;
        }
        //Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return vk::DescriptorType::eInputAttachment;
    }

    static vk::DescriptorType AsDescriptorType(COM::ClassID value) {
        switch (value) {
            case ConstantBuffer::kCID:
                return vk::DescriptorType::eUniformBuffer;
            case TextureView::kCID:
                return vk::DescriptorType::eCombinedImageSampler;
            case UnorderedTextureView::kCID:
                return vk::DescriptorType::eStorageImage;
            case UnorderedVertexBufferView::kCID:
                return vk::DescriptorType::eStorageBuffer;
            case UnorderedIndexBufferView::kCID:
                return vk::DescriptorType::eStorageTexelBuffer;
            case UnorderedStructuredBufferView::kCID:
                return vk::DescriptorType::eStorageBuffer;
            case TextureBufferView::kCID:
                return vk::DescriptorType::eUniformTexelBuffer;
            case UnorderedTextureBufferView::kCID:
                return vk::DescriptorType::eStorageTexelBuffer;
        }
        //Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return vk::DescriptorType::eInputAttachment;
    }

    static vk::Format AsFormat(GHA::Format value) {
        switch (value) {
            default:
                break;
            case Format::eUndefined:
                return vk::Format::eUndefined;
            case Format::eA2R10G10B10Unorm:
                return vk::Format::eA2R10G10B10UnormPack32;
            case Format::eR8Unorm:
                return vk::Format::eR8Unorm;
            case Format::eR8G8B8A8Srgb:
                return vk::Format::eR8G8B8A8Srgb;
            case Format::eR8G8B8A8Snorm:
                return vk::Format::eR8G8B8A8Snorm;
            case Format::eR8G8B8A8Unorm:
                return vk::Format::eR8G8B8A8Unorm;
            case Format::eB8G8R8A8Unorm:
                return vk::Format::eB8G8R8A8Unorm;
            case Format::eR16G16B16A16F:
                return vk::Format::eR16G16B16A16Sfloat;
            case Format::eR16G16B16A16I:
                return vk::Format::eR16G16B16A16Sint;
            case Format::eR16G16B16A16SNorm:
                return vk::Format::eR16G16B16A16Snorm;
            case Format::eR16G16B16A16U:
                return vk::Format::eR16G16B16A16Uint;
            case Format::eR16G16B16A16UNorm:
                return vk::Format::eR16G16B16A16Unorm;
            case Format::eR32G32B32A32F:
                return vk::Format::eR32G32B32A32Sfloat;
            case Format::eR32G32B32A32I:
                return vk::Format::eR32G32B32A32Sint;
            case Format::eR32G32B32A32U:
                return vk::Format::eR32G32B32A32Uint;
            case Format::eR32G32B32F:
                return vk::Format::eR32G32B32Sfloat;
            case Format::eR32G32B32I:
                return vk::Format::eR32G32B32Sint;
            case Format::eR32G32B32U:
                return vk::Format::eR32G32B32Uint;
            case Format::eR32G32F:
                return vk::Format::eR32G32Sfloat;
            case Format::eR32G32I:
                return vk::Format::eR32G32Sint;
            case Format::eR32G32U:
                return vk::Format::eR32G32Uint;
            case Format::eR32F:
                return vk::Format::eR32Sfloat;
            case Format::eR32I:
                return vk::Format::eR32Sint;
            case Format::eR32U:
                return vk::Format::eR32Uint;
            case Format::eR8G8B8Snorm:
                return vk::Format::eR8G8B8Snorm;
            case Format::eB8G8R8Snorm:
                return vk::Format::eB8G8R8Snorm;
            case Format::eB8G8R8A8Snorm:
                return vk::Format::eB8G8R8A8Snorm;
            case Format::eR8G8B8Unorm:
                return vk::Format::eR8G8B8Unorm;
            case Format::eB8G8R8Unorm:
                return vk::Format::eB8G8R8Unorm;
            case Format::eR16G16B16F:
                return vk::Format::eR16G16B16Sfloat;
            case Format::eR16G16B16U:
                return vk::Format::eR16G16B16Unorm;
            case Format::eBC1:
                return vk::Format::eBc1RgbaUnormBlock;
            case Format::eBC2:
                return vk::Format::eBc2UnormBlock;
            case Format::eBC3:
                return vk::Format::eBc3UnormBlock;
            case Format::eBC4:
                return vk::Format::eBc4UnormBlock;
            case Format::eBC5:
                return vk::Format::eBc5UnormBlock;
            case Format::eBC6H:
                return vk::Format::eBc6HUfloatBlock;
            case Format::eBC7:
                return vk::Format::eBc7UnormBlock;
            case Format::eASTC:
                return vk::Format::eAstc4x4UnormBlock;
            case Format::eD32F:
                return vk::Format::eD32Sfloat;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static GHA::Format AsGHAFormat(vk::Format value) {
        switch (value) {
            default:
                break;
            case vk::Format::eUndefined:
                return GHA::Format::eUndefined;
            case vk::Format::eA2R10G10B10UnormPack32:
                return GHA::Format::eA2R10G10B10Unorm;
            case vk::Format::eR8Unorm:
                return GHA::Format::eR8Unorm;
            case vk::Format::eR8G8B8A8Srgb:
                return GHA::Format::eR8G8B8A8Srgb;
            case vk::Format::eR8G8B8A8Snorm:
                return GHA::Format::eR8G8B8A8Snorm;
            case vk::Format::eR8G8B8A8Unorm:
                return GHA::Format::eR8G8B8A8Unorm;
            case vk::Format::eB8G8R8A8Unorm:
                return GHA::Format::eB8G8R8A8Unorm;
            case vk::Format::eR16G16B16A16Sfloat:
                return GHA::Format::eR16G16B16A16F;
            case vk::Format::eR16G16B16A16Sint:
                return GHA::Format::eR16G16B16A16I;
            case vk::Format::eR16G16B16A16Snorm:
                return GHA::Format::eR16G16B16A16SNorm;
            case vk::Format::eR16G16B16A16Uint:
                return GHA::Format::eR16G16B16A16U;
            case vk::Format::eR16G16B16A16Unorm:
                return GHA::Format::eR16G16B16A16UNorm;
            case vk::Format::eR32G32B32A32Sfloat:
                return GHA::Format::eR32G32B32A32F;
            case vk::Format::eR32G32B32A32Sint:
                return GHA::Format::eR32G32B32A32I;
            case vk::Format::eR32G32B32A32Uint:
                return GHA::Format::eR32G32B32A32U;
            case vk::Format::eR32G32B32Sfloat:
                return GHA::Format::eR32G32B32F;
            case vk::Format::eR32G32B32Sint:
                return GHA::Format::eR32G32B32I;
            case vk::Format::eR32G32B32Uint:
                return GHA::Format::eR32G32B32U;
            case vk::Format::eR32G32Sfloat:
                return GHA::Format::eR32G32F;
            case vk::Format::eR32G32Sint:
                return GHA::Format::eR32G32I;
            case vk::Format::eR32G32Uint:
                return GHA::Format::eR32G32U;
            case vk::Format::eR32Sfloat:
                return GHA::Format::eR32F;
            case vk::Format::eR32Sint:
                return GHA::Format::eR32I;
            case vk::Format::eR32Uint:
                return GHA::Format::eR32U;
            case vk::Format::eR8G8B8Snorm:
                return Format::eR8G8B8Snorm;
            case vk::Format::eB8G8R8Snorm:
                return Format::eB8G8R8Snorm;
            case vk::Format::eB8G8R8A8Snorm:
                return Format::eB8G8R8A8Snorm;
            case vk::Format::eR8G8B8Unorm:
                return Format::eR8G8B8Unorm;
            case vk::Format::eB8G8R8Unorm:
                return Format::eB8G8R8Unorm;
            case vk::Format::eR16G16B16Sfloat:
                return Format::eR16G16B16F;
            case vk::Format::eR16G16B16Unorm:
                return Format::eR16G16B16U;
            case vk::Format::eBc1RgbaUnormBlock:
                return Format::eBC1;
            case vk::Format::eBc2UnormBlock:
                return Format::eBC2;
            case vk::Format::eBc3UnormBlock:
                return Format::eBC3;
            case vk::Format::eBc4UnormBlock:
                return Format::eBC4;
            case vk::Format::eBc5UnormBlock:
                return Format::eBC5;
            case vk::Format::eBc6HUfloatBlock:
                return Format::eBC6H;
            case vk::Format::eBc7UnormBlock:
                return Format::eBC7;
            case vk::Format::eAstc4x4UnormBlock:
                return Format::eASTC;
            case vk::Format::eD32Sfloat:
                return Format::eD32F;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::AttachmentLoadOp AsAttachmentLoadOp(GHA::RenderTargetLoadOp value) {
        switch (value) {
            case RenderTargetLoadOp::eUndefined:
                return vk::AttachmentLoadOp::eDontCare;
            case RenderTargetLoadOp::eLoad:
                return vk::AttachmentLoadOp::eLoad;
            case RenderTargetLoadOp::eClear:
                return vk::AttachmentLoadOp::eClear;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::AttachmentStoreOp AsAttachmentStoreOp(GHA::RenderTargetWriteOp value) {
        switch (value) {
            case RenderTargetWriteOp::eUndefined:
                return vk::AttachmentStoreOp::eDontCare;
            case RenderTargetWriteOp::eWrite:
                return vk::AttachmentStoreOp::eStore;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::ShaderStageFlags AsShaderStageFlags(GHA::StageSet value) {
        vk::ShaderStageFlags flags;
        if (value & Stage::eCompute) flags |= vk::ShaderStageFlagBits::eCompute;
        if (value & Stage::eVertex) flags |= vk::ShaderStageFlagBits::eVertex;
        if (value & Stage::ePixel) flags |= vk::ShaderStageFlagBits::eFragment;
        if (value & Stage::eGeometry) flags |= vk::ShaderStageFlagBits::eGeometry;
        if (value & Stage::eHull) flags |= vk::ShaderStageFlagBits::eTessellationControl;
        if (value & Stage::eDomain) flags |= vk::ShaderStageFlagBits::eTessellationEvaluation;
        return flags;
    }

    static vk::ShaderStageFlagBits AsShaderStageFlag(GHA::Stage value) {
        switch (value) {
            default:
                break;
            case Stage::eVertex:
                return vk::ShaderStageFlagBits::eVertex;
            case Stage::eGeometry:
                return vk::ShaderStageFlagBits::eGeometry;
            case Stage::eHull:
                return vk::ShaderStageFlagBits::eTessellationControl;
            case Stage::eDomain:
                return vk::ShaderStageFlagBits::eTessellationEvaluation;
            case Stage::ePixel:
                return vk::ShaderStageFlagBits::eFragment;
            case Stage::eCompute:
                return vk::ShaderStageFlagBits::eCompute;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::SampleCountFlagBits AsSampleCountFlag(GHA::SampleMode value) {
        switch (value) {
            case SampleMode::e1:
                return vk::SampleCountFlagBits::e1;
            case SampleMode::e2:
                return vk::SampleCountFlagBits::e2;
            case SampleMode::e4:
                return vk::SampleCountFlagBits::e4;
            case SampleMode::e8:
                return vk::SampleCountFlagBits::e8;
            case SampleMode::e16:
                return vk::SampleCountFlagBits::e16;
            case SampleMode::e32:
                return vk::SampleCountFlagBits::e32;
            case SampleMode::e64:
                return vk::SampleCountFlagBits::e64;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::CullModeFlagBits AsCullModeFlag(GHA::CullMode value) {
        switch (value) {
            case CullMode::eNone:
                return vk::CullModeFlagBits::eNone;
            case CullMode::eFront:
                return vk::CullModeFlagBits::eFront;
            case CullMode::eBack:
                return vk::CullModeFlagBits::eBack;
            case CullMode::eFrontBack:
                return vk::CullModeFlagBits::eFrontAndBack;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::PolygonMode AsPolygonMode(GHA::FillMode value) {
        switch (value) {
            case FillMode::eFill:
                return vk::PolygonMode::eFill;
            case FillMode::eLine:
                return vk::PolygonMode::eLine;
            case FillMode::ePoint:
                return vk::PolygonMode::ePoint;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::BlendOp AsBlendOp(GHA::BlendOp value) {
        switch (value) {
            case BlendOp::eAdd:
                return vk::BlendOp::eAdd;
            case BlendOp::eSub:
                return vk::BlendOp::eSubtract;
            case BlendOp::eMin:
                return vk::BlendOp::eMin;
            case BlendOp::eMax:
                return vk::BlendOp::eMax;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::CompareOp AsCompareOp(GHA::CompareOp value) {
        switch (value) {
            case CompareOp::eNever:
                return vk::CompareOp::eNever;
            case CompareOp::eLess:
                return vk::CompareOp::eLess;
            case CompareOp::eEqual:
                return vk::CompareOp::eEqual;
            case CompareOp::eLessOrEqual:
                return vk::CompareOp::eLessOrEqual;
            case CompareOp::eGreater:
                return vk::CompareOp::eGreater;
            case CompareOp::eNotEqual:
                return vk::CompareOp::eNotEqual;
            case CompareOp::eGreaterOrEqual:
                return vk::CompareOp::eGreaterOrEqual;
            case CompareOp::eAlways:
                return vk::CompareOp::eAlways;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::BlendFactor AsBlendFactor(GHA::BlendFactor value) {
        switch (value) {
            case BlendFactor::eZero:
                return vk::BlendFactor::eZero;
            case BlendFactor::eOne:
                return vk::BlendFactor::eOne;
            case BlendFactor::eSrcColor:
                return vk::BlendFactor::eSrcColor;
            case BlendFactor::eOneMinusSrcColor:
                return vk::BlendFactor::eOneMinusSrcColor;
            case BlendFactor::eDstColor:
                return vk::BlendFactor::eDstColor;
            case BlendFactor::eOneMinusDstColor:
                return vk::BlendFactor::eOneMinusDstColor;
            case BlendFactor::eSrcAlpha:
                return vk::BlendFactor::eSrcAlpha;
            case BlendFactor::eOneMinusSrcAlpha:
                return vk::BlendFactor::eOneMinusSrcAlpha;
            case BlendFactor::eDstAlpha:
                return vk::BlendFactor::eDstAlpha;
            case BlendFactor::eOneMinusDstAlpha:
                return vk::BlendFactor::eOneMinusDstAlpha;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::ColorComponentFlags AsColorComponentFlags(GHA::ComponentSet value) {
        vk::ColorComponentFlags flags;
        if (value & Component::eR) flags |= vk::ColorComponentFlagBits::eR;
        if (value & Component::eG) flags |= vk::ColorComponentFlagBits::eG;
        if (value & Component::eB) flags |= vk::ColorComponentFlagBits::eB;
        if (value & Component::eA) flags |= vk::ColorComponentFlagBits::eA;
        return flags;
    }

    static vk::VertexInputRate AsInputRate(GHA::InputRate value) {
        switch (value) {
            case InputRate::eVertex:
                return vk::VertexInputRate::eVertex;
            case InputRate::eInstance:
                return vk::VertexInputRate::eInstance;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::PrimitiveTopology AsTopology(GHA::Topology value) {
        switch (value) {
            case Topology::eTriangleList:
                return vk::PrimitiveTopology::eTriangleList;
            case Topology::eLineStrip:
                return vk::PrimitiveTopology::eLineStrip;
            case Topology::eLineList:
                return vk::PrimitiveTopology::eLineList;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::Filter AsFilter(GHA::SamplerFilterMode value) {
        switch (value) {
            case SamplerFilterMode::eNearest:
                return vk::Filter::eNearest;
            case SamplerFilterMode::eLinear:
                return vk::Filter::eLinear;
            case SamplerFilterMode::eCubic:
                return vk::Filter::eCubicIMG;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::SamplerMipmapMode AsMipMapMode(GHA::SamplerMipMapMode value) {
        switch (value) {
            case SamplerMipMapMode::eNearest:
                return vk::SamplerMipmapMode::eNearest;
            case SamplerMipMapMode::eLinear:
                return vk::SamplerMipmapMode::eLinear;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::SamplerAddressMode AsAddressMode(GHA::SamplerAddressMode value) {
        switch (value) {
            case SamplerAddressMode::eRepeat:
                return vk::SamplerAddressMode::eRepeat;
            case SamplerAddressMode::eMirroredRepeat:
                return vk::SamplerAddressMode::eMirroredRepeat;
            case SamplerAddressMode::eClampToEdge:
                return vk::SamplerAddressMode::eClampToEdge;
            case SamplerAddressMode::eClampToBorder:
                return vk::SamplerAddressMode::eClampToBorder;
            case SamplerAddressMode::eMorroredClampToEdge:
                return vk::SamplerAddressMode::eMirrorClampToEdge;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::ImageType AsImageType(GHA::TextureType value) {
        switch (value) {
            case TextureType::e1D:
                return vk::ImageType::e1D;
            case TextureType::e2D:
                return vk::ImageType::e2D;
            case TextureType::e3D:
                return vk::ImageType::e3D;
            case TextureType::eCube:
                return vk::ImageType::e2D;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::ImageAspectFlagBits AsImageAspectFlag(GHA::TextureViewType value) {
        switch (value) {
            default:
                break;
            case TextureViewType::eColor:
                return vk::ImageAspectFlagBits::eColor;
            case TextureViewType::eDepth:
                return vk::ImageAspectFlagBits::eDepth;
            case TextureViewType::eStencil:
                return vk::ImageAspectFlagBits::eStencil;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::ImageAspectFlags AsImageAspectFlag(GHA::TextureUsage value) {
        switch (value) {
            case TextureUsage::eColor:
                return vk::ImageAspectFlagBits::eColor;
            case TextureUsage::eDepthStencil:
                return vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::ImageUsageFlags AsImageUsageFlags(GHA::TextureUsageSet value) {
        vk::ImageUsageFlags flags;
        flags |= vk::ImageUsageFlagBits::eSampled;
        if (value & TextureUsage::eColor) flags |= vk::ImageUsageFlagBits::eColorAttachment;
        if (value & TextureUsage::eDepthStencil) flags |= vk::ImageUsageFlagBits::eDepthStencilAttachment;
        return flags;
    }

    static vk::ImageLayout AsImageLayout(GHA::TextureLayout value) {
        switch (value) {
            case TextureLayout::eUndefined:
                return vk::ImageLayout::eUndefined;
            case TextureLayout::eColorRenderTarget:
                return vk::ImageLayout::eColorAttachmentOptimal;
            case TextureLayout::eColorRead:
                return vk::ImageLayout::eShaderReadOnlyOptimal;
            case TextureLayout::eDepthStencilRenderTarget:
                return vk::ImageLayout::eDepthStencilAttachmentOptimal;
            case TextureLayout::eDepthStencilRead:
                return vk::ImageLayout::eDepthStencilReadOnlyOptimal;
            case TextureLayout::eTransferSrc:
                return vk::ImageLayout::eTransferSrcOptimal;
            case TextureLayout::eTransferDst:
                return vk::ImageLayout::eTransferDstOptimal;
            case TextureLayout::ePresentSrc:
                return vk::ImageLayout::ePresentSrcKHR;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static void AsAccessMasks(GHA::TextureLayout before, GHA::TextureLayout after, vk::AccessFlags &src, vk::AccessFlags &dst) {
        switch (before) {
            default:
                break;
            case GHA::TextureLayout::eUndefined:
                src = static_cast<vk::AccessFlagBits>(0);
                break;
            case GHA::TextureLayout::eColorRenderTarget:
                src = vk::AccessFlagBits::eColorAttachmentWrite;
                break;
            case GHA::TextureLayout::eDepthStencilRenderTarget:
                src = vk::AccessFlagBits::eDepthStencilAttachmentWrite;
                break;
            case GHA::TextureLayout::eDepthStencilRead:
                src = vk::AccessFlagBits::eDepthStencilAttachmentRead;
                break;
            case GHA::TextureLayout::eTransferSrc:
                src = vk::AccessFlagBits::eTransferRead;
                break;
            case GHA::TextureLayout::eTransferDst:
                src = vk::AccessFlagBits::eTransferWrite;
                break;
            case GHA::TextureLayout::eColorRead:
                src = vk::AccessFlagBits::eShaderRead;
                break;
            case GHA::TextureLayout::ePresentSrc:
                src = vk::AccessFlagBits::eMemoryRead;
                break;
        }

        switch (after) {
            default:
                break;
            case GHA::TextureLayout::ePresentSrc:
                dst = vk::AccessFlagBits::eMemoryRead;
                break;
            case GHA::TextureLayout::eColorRenderTarget:
                if (before != GHA::TextureLayout::eUndefined) {
                    // TODO: Check, Barrier.srcAccessMask |= vk::AccessFlagBits::eTransferRead;
                }
                dst = vk::AccessFlagBits::eColorAttachmentWrite;
                break;
            case GHA::TextureLayout::eTransferSrc:
                if (before != GHA::TextureLayout::eUndefined) {
                    // TODO: Check, Barrier.srcAccessMask |= vk::AccessFlagBits::eTransferRead;
                }
                dst = vk::AccessFlagBits::eTransferRead;
                break;
            case GHA::TextureLayout::eDepthStencilRenderTarget:
                dst |= vk::AccessFlagBits::eDepthStencilAttachmentWrite;
                break;
            case GHA::TextureLayout::eColorRead:
                if (src == static_cast<vk::AccessFlagBits>(0) && before != GHA::TextureLayout::eUndefined) {
                    src = (vk::AccessFlagBits::eHostWrite | vk::AccessFlagBits::eTransferWrite);
                }
                dst = vk::AccessFlagBits::eShaderRead;
                break;
            case GHA::TextureLayout::eTransferDst:
                dst = vk::AccessFlagBits::eTransferWrite;
                break;
            case GHA::TextureLayout::eDepthStencilRead:
                dst = (vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eDepthStencilAttachmentRead);
                break;
        }
    }

    static vk::IndexType AsIndexType(GHA::IndexType value) {
        switch (value) {
            case IndexType::eU16:
                return vk::IndexType::eUint16;
            case IndexType::eU32:
                return vk::IndexType::eUint32;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    static vk::PipelineBindPoint AsBindPoint(GHA::BindPoint value) {
        switch (value) {
            case BindPoint::eGraphics:
                return vk::PipelineBindPoint::eGraphics;
            case BindPoint::eCompute:
                return vk::PipelineBindPoint::eCompute;
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
        return {};
    }

    /*
    template<>
    vk::DescriptorType Convert<vk::DescriptorType>(GHA::ResourceType value) {
        switch (value) {
        }
        Always("Invalid ::GHA enum to ::Vulkan adapter conversion");
    }
    */
}