#include <GHA/Vulkan/Device.h>
#include <GHA/Vulkan/Adapter.h>
#include <GHA/Vulkan/Queue.h>
#include <GHA/Vulkan/Allocator.h>
#include <GHA/Vulkan/DescriptorAllocator.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <Core/Memory.h>
#include <Core/Map.h>
#include <GHA/Vulkan/Swapchain.h>
#include <GHA/Vulkan/CommandPool.h>
#include <GHA/Vulkan/CommandList.h>
#include <GHA/Vulkan/Fence.h>
#include <GHA/Vulkan/Frame.h>
#include <GHA/Vulkan/IndexBuffer.h>
#include <GHA/Vulkan/Pass.h>
#include <GHA/Vulkan/Pipeline.h>
#include <GHA/Vulkan/PipelineLayout.h>
#include <GHA/Vulkan/DescriptorSetLayout.h>
#include <GHA/Vulkan/DescriptorSet.h>
#include <GHA/Vulkan/Sampler.h>
#include <GHA/Vulkan/StructuredBuffer.h>
#include <GHA/Vulkan/Texture.h>
#include <GHA/Vulkan/TextureBuffer.h>
#include <GHA/Vulkan/TextureView.h>
#include <GHA/Vulkan/VertexBuffer.h>
#include <GHA/Vulkan/Shader.h>
#include <GHA/ShaderBlob.h>
#include <GHA/Vulkan/IndexBufferView.h>
#include <GHA/Vulkan/UnorderedIndexBufferView.h>
#include <GHA/Vulkan/StructuredBufferView.h>
#include <GHA/Vulkan/UnorderedStructuredBufferView.h>
#include <GHA/Vulkan/UnorderedTextureView.h>
#include <GHA/Vulkan/TextureBufferView.h>
#include <GHA/Vulkan/UnorderedTextureBufferView.h>
#include <GHA/Vulkan/UnorderedVertexBufferView.h>
#include <GHA/Vulkan/Semaphore.h>
#include <GHA/Vulkan/ShaderNILCompiler.h>
#include <GHA/Vulkan/ResourceStager.h>
#include <GHA/Vulkan/CompiledShaderBlob.h>
#include <GHA/Vulkan/ConstantBuffer.h>

#undef CreateSemaphore

DJINN_NS3(GHA, Vulkan);

Device::Device(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IDevice(registry, outer, host) {
}


COM::Result Device::Initialize(const GHA::DeviceDesc &desc, GHA::IAdapter *adapter, vk::Instance &instance) {
    this->adapter = static_cast<Adapter *>(adapter);
    this->instance = instance;
    this->desc = desc;

    // Attempt to create
    if (!GetExtensions() || !CreateDevice() || !CreateQueues()) {
        return COM::kError;
    }

    // Create memory allocator
    if (!registry->CreateClass(this, &allocator) || !allocator->Initialize(adapter, device)) {
        ErrorLog(this).Write("Failed to create vulkan allocator");
        return COM::kError;
    }

    // Create descriptor allocator
    if (!registry->CreateClass(this, &descriptorAllocator) || !descriptorAllocator->Initialize(device)) {
        ErrorLog(this).Write("Failed to create vulkan descriptor allocator");
        return COM::kError;
    }

    // Create pipeline cache
    vk::PipelineCacheCreateInfo createInfo;
    if (vk::CheckedResult result = device.createPipelineCache(&createInfo, nullptr, &pipelineCache)) {
        ErrorLog(this).Write("Failed to create vulkan pipeline cache: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Transfer queue priority:
    // 1. Dedicated transfer
    // 2. Non primary transfer queue
    // 3. Primary queue
    GHA::Vulkan::Queue *transferQueue;
    if (!(transferQueue = queues.FirstOrDefault([&](GHA::Vulkan::Queue *q) { return q->GetType() == GHA::QueueType::eTransfer; }))) {
        if (!(transferQueue = queues.FirstOrDefault([&](GHA::Vulkan::Queue *q) { return q != queues[0] && q->GetType() == GHA::QueueType::eTransfer; }))) {
            transferQueue = queues[0];
        }
    }

    // Create stager
    if (!registry->CreateClass(this, &resourceStager) || !resourceStager->Initialize(device, allocator, transferQueue)) {
        ErrorLog(this).Write("Failed to create vulkan resource stager");
        return COM::kError;
    }

    // ...
    return COM::kOK;
}

COM::Result Device::GetQueues(UInt32 &count, Queue **queues) {
    if (queues) {
        Core::Memory::Copy(queues, this->queues.Ptr(), count * sizeof(Queue *));
    } else {
        count = static_cast<UInt32>(this->queues.Size());
    }
    return COM::kOK;
}

COM::Result Device::CreateSwapchain(COM::IUnknown* outer, const GHA::SwapchainDesc &desc, GHA::ISwapchain **out) {
    // Create swapchain
    Swapchain *swapchain;
    if (!registry->CreateClass(outer, &swapchain) || !swapchain->Initialize(desc, adapter, device, instance)) {
        ErrorLog(this).Write("Failed to create vulkan swapchain");
        return COM::kError;
    }

    *out = swapchain;
    return COM::kOK;
}

COM::Result Device::CreateCommandPool(COM::IUnknown* outer, const PipelineScheduler::CommandPoolDesc &desc, Queue *queue, CommandPool **out) {
    // Create command pool
    CommandPool *pool;
    if (!registry->CreateClass(outer, &pool) || !pool->Initialize(desc, queue, device)) {
        ErrorLog(this).Write("Failed to create vulkan command pool");
        return COM::kError;
    }

    *out = pool;
    return COM::kOK;
}

COM::Result Device::CreateCommandList(COM::IUnknown* outer, const PipelineScheduler::CommandListDesc &desc, CommandPool *pool, CommandList **out) {
    // Create command list
    CommandList *list;
    if (!registry->CreateClass(outer, &list) || !list->Initialize(desc, device, pool->GetPool())) {
        ErrorLog(this).Write("Failed to create vulkan command list");
        return COM::kError;
    }

    *out = list;
    return COM::kOK;
}

COM::Result Device::CreateFence(COM::IUnknown* outer, const PipelineScheduler::FenceDesc &desc, Fence **out) {
    // Create fence
    Fence *fence;
    if (!registry->CreateClass(outer, &fence) || !fence->Initialize(desc, device)) {
        ErrorLog(this).Write("Failed to create vulkan fence");
        return COM::kError;
    }

    *out = fence;
    return COM::kOK;
}

COM::Result Device::CreateFrame(COM::IUnknown* outer, const FrameDesc &desc, Pass *pass, Frame **out) {
    // Create frame
    Frame *frame;
    if (!registry->CreateClass(outer, &frame) || !frame->Initialize(desc, pass, device)) {
        ErrorLog(this).Write("Failed to create vulkan frame");
        return COM::kError;
    }

    *out = frame;
    return COM::kOK;
}

COM::Result Device::CreateIndexBuffer(COM::IUnknown* outer, const GHA::IndexBufferDesc &desc, GHA::IIndexBuffer **out) {
    // Create buffer
    IndexBuffer *ib;
    if (!registry->CreateClass(outer, &ib) || !ib->Initialize(desc, device, allocator)) {
        ErrorLog(this).Write("Failed to create vulkan index buffer");
        return COM::kError;
    }

    *out = ib;
    return COM::kOK;
}

COM::Result Device::CreateConstantBuffer(COM::IUnknown* outer, const ConstantBufferDesc &desc, IConstantBuffer **out) {
    // Create buffer
    ConstantBuffer *ib;
    if (!registry->CreateClass(outer, &ib) || !ib->Initialize(desc, device, allocator)) {
        ErrorLog(this).Write("Failed to create vulkan constant buffer");
        return COM::kError;
    }

    *out = ib;
    return COM::kOK;
}

COM::Result Device::CreatePass(COM::IUnknown* outer, const PipelineScheduler::PassDesc &desc, Pass **out) {
    // Create pass
    Pass *pass;
    if (!registry->CreateClass(outer, &pass) || !pass->Initialize(desc, device)) {
        ErrorLog(this).Write("Failed to create vulkan pass");
        return COM::kError;
    }

    *out = pass;
    return COM::kOK;
}

COM::Result Device::CreatePipelineLayout(COM::IUnknown* outer, const PipelineLayoutDesc &desc, PipelineLayout **out) {
    // Create pipeline
    PipelineLayout *pipelineLayout;
    if (!registry->CreateClass(outer, &pipelineLayout) || !pipelineLayout->Initialize(desc, device)) {
        ErrorLog(this).Write("Failed to create vulkan pipeline layout");
        return COM::kError;
    }

    *out = pipelineLayout;
    return COM::kOK;
}

COM::Result Device::CreatePipeline(COM::IUnknown* outer, const PipelineScheduler::PipelineDesc &desc, PipelineLayout *layout, Pass *pass, Pipeline **out) {
    // Create pipeline
    Pipeline *pipeline;
    if (!registry->CreateClass(outer, &pipeline) || !pipeline->Initialize(desc, layout, pass, pipelineCache, adapter->GetCapabilities(), device)) {
        ErrorLog(this).Write("Failed to create vulkan pipeline");
        return COM::kError;
    }

    *out = pipeline;
    return COM::kOK;
}

COM::Result Device::CreateDescriptorSetLayout(COM::IUnknown* outer, const PipelineScheduler::DescriptorSetLayoutDesc &desc, DescriptorSetLayout **out) {
    // Create pipeline
    DescriptorSetLayout *groupLayout;
    if (!registry->CreateClass(outer, &groupLayout) || !groupLayout->Initialize(desc, device)) {
        ErrorLog(this).Write("Failed to create vulkan descriptor set layout");
        return COM::kError;
    }

    *out = groupLayout;
    return COM::kOK;
}

COM::Result Device::CreateDescriptorSet(COM::IUnknown* outer, const DescriptorSetUpdateDesc &desc, DescriptorSetLayout *layout, DescriptorSet **out) {
    // Create resource group
    DescriptorSet *group;
    if (!registry->CreateClass(outer, &group) || !group->Initialize(desc, layout, descriptorAllocator, device)) {
        ErrorLog(this).Write("Failed to create vulkan descriptor set");
        return COM::kError;
    }

    *out = group;
    return COM::kOK;
}

COM::Result Device::CreateSampler(COM::IUnknown* outer, const GHA::SamplerDesc &desc, GHA::ISampler **out) {
    // Create sampler
    Sampler *sampler;
    if (!registry->CreateClass(outer, &sampler) || !sampler->Initialize(desc, device)) {
        ErrorLog(this).Write("Failed to create vulkan sampler");
        return COM::kError;
    }

    *out = sampler;
    return COM::kOK;
}

COM::Result Device::CreateShader(COM::IUnknown* outer, const ShaderDesc &desc, IShader **out) {
    // Must use vulkan shader compiler
    if (!desc.blob->compiledBlobs.Contains(ShaderNILCompiler::kCID)) {
        ErrorLog(this).Write("Failed to create vulkan shader, shader blob not compatible due to missing NIL compiler, recompilation needed.");
        return COM::kError;
    }

    // Get blob
    auto &blob = desc.blob->compiledBlobs[ShaderNILCompiler::kCID];

    // Create shader
    Shader *shader;
    if (!registry->CreateClass(outer, &shader) || !shader->Initialize(desc, static_cast<CompiledShaderBlob*>(blob), device)) {
        ErrorLog(this).Write("Failed to create vulkan shader");
        return COM::kError;
    }

    *out = shader;
    return COM::kOK;
}

COM::Result Device::CreateStructuredBuffer(COM::IUnknown* outer, const GHA::StructuredBufferDesc &desc, GHA::IStructuredBuffer **out) {
    // Create pipeline
    StructuredBuffer *buffer;
    if (!registry->CreateClass(outer, &buffer) || !buffer->Initialize(desc, device, allocator)) {
        ErrorLog(this).Write("Failed to create vulkan sampler");
        return COM::kError;
    }

    *out = buffer;
    return COM::kOK;
}

COM::Result Device::CreateTexture(COM::IUnknown* outer, const GHA::TextureDesc &desc, GHA::ITexture **out) {
    // Create texture
    Texture *texture;
    if (!registry->CreateClass(outer, &texture) || !texture->Initialize(desc, device, allocator, adapter->GetCapabilities())) {
        ErrorLog(this).Write("Failed to create vulkan texture");
        return COM::kError;
    }

    *out = texture;
    return COM::kOK;
}

COM::Result Device::CreateTextureBuffer(COM::IUnknown* outer, const GHA::TextureBufferDesc &desc, GHA::ITextureBuffer **out) {
    // Create texture
    TextureBuffer *buffer;
    if (!registry->CreateClass(outer, &buffer) || !buffer->Initialize(desc, adapter->GetCapabilities(), device, allocator)) {
        ErrorLog(this).Write("Failed to create vulkan texture");
        return COM::kError;
    }

    *out = buffer;
    return COM::kOK;
}

COM::Result Device::CreateTextureView(COM::IUnknown* outer, const GHA::TextureViewDesc &desc, ITexture *texture, TextureView **out) {
    // Create texture
    TextureView *view;
    if (!registry->CreateClass(outer, &view) || !view->Initialize(desc, device, texture)) {
        ErrorLog(this).Write("Failed to create vulkan texture view");
        return COM::kError;
    }

    *out = view;
    return COM::kOK;
}

COM::Result Device::CreateVertexBuffer(COM::IUnknown* outer, const GHA::VertexBufferDesc &desc, IVertexBuffer **out) {
    // Create texture
    VertexBuffer *buffer;
    if (!registry->CreateClass(outer, &buffer) || !buffer->Initialize(desc, device, allocator)) {
        ErrorLog(this).Write("Failed to create vulkan vertex buffer");
        return COM::kError;
    }

    *out = buffer;
    return COM::kOK;
}

COM::Result Device::AddExtension(const char *name) {
    if (!extensionPropertiesMap.Contains(name)) {
        ErrorLog(this).Write("Vulkan device extension '", name, "not supported");
        return COM::kError;
    }
    extensions.Add(name);
    return COM::kOK;
}

COM::Result Device::AddLayer(const char *name) {
    if (!layerPropertiesMap.Contains(name)) {
        ErrorLog(this).Write("Vulkan device layer '", name, "' not supported");
        return COM::kError;
    }
    layers.Add(name);
    return COM::kOK;
}

const GHA::DeviceDesc &Device::GetDesc() {
    return desc;
}

COM::Result Device::GetExtensions() {
// Get layer count
    UInt32 layerCount;
    if (vk::CheckedResult result = adapter->GetPhysicalDevice().enumerateDeviceLayerProperties(&layerCount, nullptr)) {
        ErrorLog(this).Write("Failed to get vulkan layer count: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Get layers
    layerProperties.Resize(layerCount);
    if (vk::CheckedResult result = adapter->GetPhysicalDevice().enumerateDeviceLayerProperties(&layerCount, layerProperties.Ptr())) {
        ErrorLog(this).Write("Failed to get vulkan layers: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Map layers
    layerProperties.Foreach([&](const vk::LayerProperties &property) { layerPropertiesMap[property.layerName] = property; });

    // Get extension count
    UInt32 extensionCount;
    if (vk::CheckedResult result = adapter->GetPhysicalDevice().enumerateDeviceExtensionProperties(nullptr, &extensionCount, nullptr)) {
        ErrorLog(this).Write("Failed to get vulkan extension count: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Get extensions
    extensionProperties.Resize(extensionCount);
    if (vk::CheckedResult result = adapter->GetPhysicalDevice().enumerateDeviceExtensionProperties(nullptr, &extensionCount, extensionProperties.Ptr())) {
        ErrorLog(this).Write("Failed to get vulkan extensions: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Map extensions
    extensionProperties.Foreach([&](const vk::ExtensionProperties &property) { extensionPropertiesMap[property.extensionName] = property; });

    // OK
    return COM::kOK;
}

COM::Result Device::CreateDevice() {
    // Get queue count
    std::uint32_t queueCount;
    adapter->GetPhysicalDevice().getQueueFamilyProperties(&queueCount, nullptr);

    // Get queue families
    queueFamilyProperties.Resize(queueCount);
    adapter->GetPhysicalDevice().getQueueFamilyProperties(&queueCount, queueFamilyProperties.Ptr());

    // Queue descriptions
    Core::Array<vk::DeviceQueueCreateInfo> queueCreateInfos;

    // Default queue priority
    Core::Array<float> queuePriorities(128);
    for (float &priority : queuePriorities) {
        priority = 1.0f;
    }

    // Create queues
    UInt32 queueIndex = 0;
    for (vk::QueueFamilyProperties &queueFamilyProperty : queueFamilyProperties) {
        vk::DeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.queueFamilyIndex = queueIndex++;
        queueCreateInfo.queueCount = queueFamilyProperty.queueCount;
        queueCreateInfo.pQueuePriorities = queuePriorities.Ptr();
        queueCreateInfos.Add(queueCreateInfo);
    }

    // Core extensions
    if (!AddExtension("VK_KHR_swapchain")) {
        return COM::kError;
    }

    // Debug extension
    if (desc.flags & GHA::DeviceFlag::eDebug) {
        if (!AddLayer("VK_LAYER_LUNARG_standard_validation")) {
            return COM::kError;
        }

        // Debug marker is optional
        if (extensionPropertiesMap.Contains("VK_EXT_debug_marker")) {
            if (!AddExtension("VK_EXT_debug_marker")) {
                return COM::kError;
            }
        } else {
            WarningLog(this).Write("Vulkan device debug flag requested but debug marker extension not supported, external IDEs may not function optimally");
        }
    }

    // Certain device features aren't implemented in the Linux MESA drivers (i.e., tessellation).  If on Linux
    // with the MESA drivers, enabling these features will cause device creation to fail. -- NCY, 5.25.2018

    // Describe features
    vk::PhysicalDeviceFeatures enabledFeatures;
    enabledFeatures.robustBufferAccess = 0;                             // Supported
    enabledFeatures.fullDrawIndexUint32 = 1;                            // Supported
    enabledFeatures.imageCubeArray = 1;                                 // Supported
    enabledFeatures.independentBlend = 1;                               // Supported
    enabledFeatures.geometryShader = 1;                                 // Supported
    enabledFeatures.tessellationShader = 1;                             // Supported
    enabledFeatures.sampleRateShading = 1;                              // Supported
    enabledFeatures.dualSrcBlend = 1;                                   // Supported
    enabledFeatures.logicOp = 1;                                        // Supported
    enabledFeatures.multiDrawIndirect = 1;                              // Supported
    enabledFeatures.drawIndirectFirstInstance = 1;                      // Supported
    enabledFeatures.depthClamp = 1;                                     // Supported
    enabledFeatures.depthBiasClamp = 1;                                 // Supported
    enabledFeatures.fillModeNonSolid = 1;                               // Supported
    enabledFeatures.depthBounds = 1;                                    // Supported
    enabledFeatures.wideLines = 1;                                      // Supported
    enabledFeatures.largePoints = 1;                                    // Supported
    enabledFeatures.alphaToOne = 1;                                     // Supported
    enabledFeatures.multiViewport = 1;                                  // Supported
    enabledFeatures.samplerAnisotropy = 1;                              // Supported
    enabledFeatures.textureCompressionETC2 = 0;                         // Supported
    enabledFeatures.textureCompressionASTC_LDR = 0;                     // Supported
    enabledFeatures.textureCompressionBC = 1;                           // Supported
    enabledFeatures.occlusionQueryPrecise = 1;                          // Supported
    enabledFeatures.pipelineStatisticsQuery = 1;                        // Supported
    enabledFeatures.vertexPipelineStoresAndAtomics = 1;                 // Supported
    enabledFeatures.fragmentStoresAndAtomics = 1;                       // Supported
    enabledFeatures.shaderTessellationAndGeometryPointSize = 1;         // Supported
    enabledFeatures.shaderImageGatherExtended = 1;                      // Supported
    enabledFeatures.shaderStorageImageExtendedFormats = 1;              // Supported
//    enabledFeatures.shaderStorageImageMultisample = 1;                // Not supported
    enabledFeatures.shaderStorageImageReadWithoutFormat = 1;            // Supported
    enabledFeatures.shaderStorageImageWriteWithoutFormat = 1;           // Supported
    enabledFeatures.shaderUniformBufferArrayDynamicIndexing = 1;        // Supported
    enabledFeatures.shaderSampledImageArrayDynamicIndexing = 1;         // Supported
    enabledFeatures.shaderStorageBufferArrayDynamicIndexing = 1;        // Supported
    enabledFeatures.shaderStorageImageArrayDynamicIndexing = 1;         // Supported.
    enabledFeatures.shaderClipDistance = 1;                             // Supported
    enabledFeatures.shaderCullDistance = 1;                             // Supported
    enabledFeatures.shaderFloat64 = 1;                                  // Supported
    enabledFeatures.shaderInt64 = 1;                                    // Supported
    enabledFeatures.shaderInt16 = 0;                                    // Supported.
//    enabledFeatures.shaderResourceResidency = 1;                      // Not supported
//    enabledFeatures.shaderResourceMinLod = 1;                         // Not supported
    enabledFeatures.sparseBinding = 1;                                  // Supported
//    enabledFeatures.sparseResidencyBuffer = 1;                        // Not supported
//   enabledFeatures.sparseResidencyImage2D = 1;                        // Not supported
//    enabledFeatures.sparseResidencyImage3D = 1;                       // Not supported
//    enabledFeatures.sparseResidency2Samples = 1;                      // Not supported
//    enabledFeatures.sparseResidency4Samples = 1;                      // Not supported
//    enabledFeatures.sparseResidency8Samples = 1;                      // Not supported
//    enabledFeatures.sparseResidency16Samples = 1;                     // Not supported
//    enabledFeatures.sparseResidencyAliased = 1;                       // Not supported
    enabledFeatures.variableMultisampleRate = 1;                        // Supported
    enabledFeatures.inheritedQueries = 1;                               // Supported

    // Create device
    vk::DeviceCreateInfo createInfo;
    createInfo.queueCreateInfoCount = static_cast<UInt32>(queueCreateInfos.Size());
    createInfo.pQueueCreateInfos = queueCreateInfos.Ptr();
    createInfo.pEnabledFeatures = &enabledFeatures;
    createInfo.ppEnabledExtensionNames = extensions.Ptr();
    createInfo.ppEnabledLayerNames = layers.Ptr();
    createInfo.enabledExtensionCount = static_cast<UInt32>(extensions.Size());
    createInfo.enabledLayerCount = static_cast<UInt32>(layers.Size());
    if (vk::CheckedResult result = adapter->GetPhysicalDevice().createDevice(&createInfo, nullptr, &device)) {
        ErrorLog(this).Write("Failed to create vulkan device: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Diagnostic
    InfoLog(this).Write("Initialized");

    // OK
    return COM::kOK;
}

COM::Result Device::CreateQueues() {
    // Diagnostic
    Core::Map<GHA::QueueType, UInt32> queueCounts;

    // Create queues
    for (UInt32 queueFamilyIndex = 0; queueFamilyIndex < static_cast<UInt32>(queueFamilyProperties.Size()); queueFamilyIndex++) {
        for (UInt32 i = 0; i < queueFamilyProperties[queueFamilyIndex].queueCount; i++) {
            // Get vulkan queue
            vk::Queue vkQueue;
            device.getQueue(queueFamilyIndex, i, &vkQueue);
            if (!vkQueue) {
                ErrorLog(this).Write("Failed to get vulkan queue");
                return COM::kError;
            }

            // Create queue
            Queue *queue;
            if (!registry->CreateClass(outer, &queue) || !queue->Initialize(queueFamilyProperties[queueFamilyIndex], queueFamilyIndex, i, vkQueue)) {
                ErrorLog(this).Write("Failed to create vulkan queue");
                return COM::kError;
            }

            // Diagnostic
            GHA::QueueTypeSet type = queue->GetType();
            if (type & GHA::QueueType::eGraphics) {
                queueCounts[GHA::QueueType::eGraphics]++;
            }
            if (type & GHA::QueueType::eCompute) {
                queueCounts[GHA::QueueType::eCompute]++;
            }
            if (type & GHA::QueueType::eTransfer) {
                queueCounts[GHA::QueueType::eTransfer]++;
            }

            // ...
            queues.Add(queue);
        }
    }

    // Diagnostic
    InfoLog(this).Write(
            "Created ", queues.Size(), " vulkan queues [",
            queueCounts[GHA::QueueType::eGraphics], " graphic(s), ",
            queueCounts[GHA::QueueType::eCompute], " compute(s), ",
            queueCounts[GHA::QueueType::eTransfer], " transfer(s)]"
    );

    // OK
    return COM::kOK;
}

GHA::IAdapter *Device::GetAdapter() {
    return adapter;
}

COM::Result Device::CreateIndexBufferView(COM::IUnknown* outer, IIndexBuffer *indexBuffer, IndexBufferView **out) {
    // Create buffer
    IndexBufferView *ib;
    if (!registry->CreateClass(outer, &ib) || !ib->Initialize(device, indexBuffer)) {
        ErrorLog(this).Write("Failed to create vulkan index buffer view");
        return COM::kError;
    }

    *out = ib;
    return COM::kOK;
}

COM::Result Device::CreateUnorderedIndexBufferView(COM::IUnknown* outer, IndexBufferView *indexBufferView, UnorderedIndexBufferView **out) {
    // Create buffer
    UnorderedIndexBufferView *ib;
    if (!registry->CreateClass(outer, &ib) || !ib->Initialize(device, indexBufferView)) {
        ErrorLog(this).Write("Failed to create vulkan unordered index buffer view");
        return COM::kError;
    }

    *out = ib;
    return COM::kOK;
}

COM::Result Device::CreateStructuredBufferView(COM::IUnknown* outer, IStructuredBuffer *structuredBuffer, StructuredBufferView **out) {
    // Create buffer
    StructuredBufferView *ib;
    if (!registry->CreateClass(outer, &ib) || !ib->Initialize(device, structuredBuffer)) {
        ErrorLog(this).Write("Failed to create vulkan structured buffer view");
        return COM::kError;
    }

    *out = ib;
    return COM::kOK;
}

COM::Result Device::CreateUnorederedStructuredBufferView(COM::IUnknown* outer, StructuredBufferView *structuredBufferView, UnorderedStructuredBufferView **out) {
    // Create buffer
    UnorderedStructuredBufferView *ib;
    if (!registry->CreateClass(outer, &ib) || !ib->Initialize(device, structuredBufferView)) {
        ErrorLog(this).Write("Failed to create vulkan unordered structured buffer view");
        return COM::kError;
    }

    *out = ib;
    return COM::kOK;
}

COM::Result Device::CreateUnorderedTextureView(COM::IUnknown* outer, TextureView *view, UnorderedTextureView **out) {
    // Create buffer
    UnorderedTextureView *ib;
    if (!registry->CreateClass(outer, &ib) || !ib->Initialize(device, view)) {
        ErrorLog(this).Write("Failed to create vulkan unordered texture view");
        return COM::kError;
    }

    *out = ib;
    return COM::kOK;
}

COM::Result Device::CreateTextureBufferView(COM::IUnknown* outer, const TextureBufferViewDesc &desc, ITextureBuffer *textureBuffer, TextureBufferView **out) {
    // Create buffer
    TextureBufferView *ib;
    if (!registry->CreateClass(outer, &ib) || !ib->Initialize(desc, device, textureBuffer)) {
        ErrorLog(this).Write("Failed to create vulkan texture buffer view");
        return COM::kError;
    }

    *out = ib;
    return COM::kOK;
}

COM::Result Device::CreateUnorderedTextureBufferView(COM::IUnknown* outer, TextureBufferView *textureBufferView, UnorderedTextureBufferView **out) {
    // Create buffer
    UnorderedTextureBufferView *ib;
    if (!registry->CreateClass(outer, &ib) || !ib->Initialize(device, textureBufferView)) {
        ErrorLog(this).Write("Failed to create vulkan unordered texture buffer view");
        return COM::kError;
    }

    *out = ib;
    return COM::kOK;
}

COM::Result Device::CreateUnorderedVertexBufferView(COM::IUnknown* outer, IVertexBuffer *vertexBuffer, UnorderedVertexBufferView **out) {
    // Create buffer
    UnorderedVertexBufferView *ib;
    if (!registry->CreateClass(outer, &ib) || !ib->Initialize(device, vertexBuffer)) {
        ErrorLog(this).Write("Failed to create vulkan unordered vertex buffer view");
        return COM::kError;
    }

    *out = ib;
    return COM::kOK;
}

COM::Result Device::CreateSemaphore(COM::IUnknown* outer, Semaphore **out) {
    // Create buffer
    Semaphore *ib;
    if (!registry->CreateClass(outer, &ib) || !ib->Initialize(device)) {
        ErrorLog(this).Write("Failed to create vulkan semaphore");
        return COM::kError;
    }

    *out = ib;
    return COM::kOK;
}

IResourceStager* Device::GetResourceStager() {
    return resourceStager;
}

IDeviceMemory *Device::GetMemory() {
    return allocator;
}

COM::Result Device::Released() {
    device.destroyPipelineCache(pipelineCache, nullptr);
    device.destroy(nullptr);

    // OK
    return COM::kOK;
}
