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

#include <GHA/IDevice.h>
#include <Core/HashMap.h>
#include <vulkan/vulkan.ext.hpp>
#include <Core/String.h>
#include <GHA/PipelineScheduler/DescriptorSetUpdateDesc.h>
#include <GHA/PipelineScheduler/CommandPoolDesc.h>
#include <GHA/PipelineScheduler/CommandListDesc.h>
#include <GHA/PipelineScheduler/FenceDesc.h>
#include <GHA/PipelineScheduler/FrameDesc.h>
#include <GHA/PipelineScheduler/PassDesc.h>
#include <GHA/PipelineScheduler/PipelineDesc.h>
#include <GHA/TextureViewDesc.h>
#include <GHA/PipelineScheduler/DescriptorSetLayoutDesc.h>
#include "FrameDesc.h"
#include "PipelineLayoutDesc.h"
#include "DescriptorSetUpdateDesc.h"

#undef CreateSemaphore

namespace Djinn::GHA {
    class IAdapter;
}

namespace Djinn::GHA::Vulkan {
    class Queue;
    class Adapter;
    class Allocator;
    class DescriptorAllocator;
    class DescriptorSet;
    class DescriptorSetLayout;
    class Pipeline;
    class PipelineLayout;
    class Queue;
    class Semaphore;
    class CommandList;
    class CommandPool;
    class Fence;
    class Pass;
    class Frame;
    class TextureView;
    class UnorderedTextureView;
    class StructuredBufferView;
    class UnorderedStructuredBufferView;
    class TextureBufferView;
    class UnorderedTextureBufferView;
    class IndexBufferView;
    class UnorderedIndexBufferView;
    class VertexBufferView;
    class UnorderedVertexBufferView;
    class ResourceStager;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN Device : public IDevice {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Device(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const DeviceDesc &desc, IAdapter *adapter, vk::Instance &instance);

        /// Overrides
        const DeviceDesc &GetDesc() override;
        IAdapter *GetAdapter() override;
        virtual IDeviceMemory *GetMemory() override;
        COM::Result GetQueues(UInt32 &count, Queue **queues);
        IResourceStager *GetResourceStager() override;
        COM::Result CreateConstantBuffer(COM::IUnknown *outer, const ConstantBufferDesc &desc, IConstantBuffer **out) override;
        COM::Result CreateSemaphore(COM::IUnknown *outer, Semaphore **out);
        COM::Result CreateSwapchain(COM::IUnknown *outer, const SwapchainDesc &desc, ISwapchain **out) override;
        COM::Result CreateCommandPool(COM::IUnknown *outer, const PipelineScheduler::CommandPoolDesc &desc, Queue *queue, CommandPool **out);
        COM::Result CreateCommandList(COM::IUnknown *outer, const PipelineScheduler::CommandListDesc &desc, CommandPool *pool, CommandList **out);
        COM::Result CreateFence(COM::IUnknown *outer, const PipelineScheduler::FenceDesc &desc, Fence **out);
        COM::Result CreateFrame(COM::IUnknown *outer, const FrameDesc &desc, Pass *pass, Frame **out);
        COM::Result CreateIndexBuffer(COM::IUnknown *outer, const IndexBufferDesc &desc, IIndexBuffer **out) override;
        COM::Result CreatePass(COM::IUnknown *outer, const PipelineScheduler::PassDesc &desc, Pass **out);
        COM::Result CreatePipelineLayout(COM::IUnknown *outer, const PipelineLayoutDesc &desc, PipelineLayout **out);
        COM::Result CreateDescriptorSetLayout(COM::IUnknown *outer, const PipelineScheduler::DescriptorSetLayoutDesc &desc, DescriptorSetLayout **out);
        COM::Result CreatePipeline(COM::IUnknown *outer, const PipelineScheduler::PipelineDesc &desc, PipelineLayout *layout, Pass *pass, Pipeline **out);
        COM::Result CreateDescriptorSet(COM::IUnknown *outer, const DescriptorSetUpdateDesc &desc, DescriptorSetLayout *layout, DescriptorSet **out);
        COM::Result CreateSampler(COM::IUnknown *outer, const SamplerDesc &desc, ISampler **out) override;
        COM::Result CreateShader(COM::IUnknown *outer, const ShaderDesc &desc, IShader **out) override;
        COM::Result CreateStructuredBuffer(COM::IUnknown *outer, const StructuredBufferDesc &desc, IStructuredBuffer **out) override;
        COM::Result CreateTexture(COM::IUnknown *outer, const TextureDesc &desc, ITexture **out) override;
        COM::Result CreateTextureBuffer(COM::IUnknown *outer, const TextureBufferDesc &desc, ITextureBuffer **out) override;
        COM::Result CreateTextureView(COM::IUnknown *outer, const TextureViewDesc &desc, ITexture *texture, TextureView **out);
        COM::Result CreateVertexBuffer(COM::IUnknown *outer, const VertexBufferDesc &desc, IVertexBuffer **out) override;
        COM::Result CreateIndexBufferView(COM::IUnknown *outer, IIndexBuffer *indexBuffer, IndexBufferView **out);
        COM::Result CreateUnorderedIndexBufferView(COM::IUnknown *outer, IndexBufferView *indexBufferView, UnorderedIndexBufferView **out);
        COM::Result CreateStructuredBufferView(COM::IUnknown *outer, IStructuredBuffer *structuredBuffer, StructuredBufferView **out);
        COM::Result CreateUnorederedStructuredBufferView(COM::IUnknown *outer, StructuredBufferView *structuredBufferView, UnorderedStructuredBufferView **out);
        COM::Result CreateUnorderedTextureView(COM::IUnknown *outer, TextureView *view, UnorderedTextureView **out);
        COM::Result CreateTextureBufferView(COM::IUnknown *outer, const TextureBufferViewDesc &desc, ITextureBuffer *textureBuffer, TextureBufferView **out);
        COM::Result CreateUnorderedTextureBufferView(COM::IUnknown *outer, TextureBufferView *textureBufferView, UnorderedTextureBufferView **out);
        COM::Result CreateUnorderedVertexBufferView(COM::IUnknown *outer, IVertexBuffer *vertexBuffer, UnorderedVertexBufferView **out);

        /// Add extension
        /// \param name
        COM::Result AddExtension(const char *name);

        /// Add layer
        /// \param name
        COM::Result AddLayer(const char *name);

        /// Get vk device
        /// \return
        vk::Device GetDevice() const {
            return device;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        COM::Result GetExtensions();
        COM::Result CreateDevice();
        COM::Result CreateQueues();

    private:
        Core::AtomicCounter uid{0};
        Adapter *adapter;
        DeviceDesc desc;
        vk::Device device;
        vk::Instance instance;
        Core::Array<vk::QueueFamilyProperties> queueFamilyProperties;
        Core::Array<const char *> extensions;
        Core::Array<const char *> layers;
        Core::Array<vk::ExtensionProperties> extensionProperties;
        Core::Array<vk::LayerProperties> layerProperties;
        Core::HashMap<Core::String, vk::ExtensionProperties> extensionPropertiesMap;
        Core::HashMap<Core::String, vk::LayerProperties> layerPropertiesMap;
        Core::Array<Queue *> queues;
        Allocator *allocator;
        DescriptorAllocator *descriptorAllocator;
        vk::PipelineCache pipelineCache;
        ResourceStager *resourceStager;
    };
}