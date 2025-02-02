#include <GHA/Vulkan/DescriptorSet.h>
#include <GHA/Vulkan/DescriptorSetLayout.h>
#include <GHA/Vulkan/DescriptorAllocator.h>
#include <GHA/Vulkan/Texture.h>
#include <GHA/Vulkan/TextureView.h>
#include <GHA/Vulkan/VertexBuffer.h>
#include <GHA/Vulkan/IndexBuffer.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <GHA/Vulkan/ConstantBuffer.h>
#include <GHA/Vulkan/Sampler.h>
#include <GHA/Vulkan/UnorderedTextureView.h>
#include <GHA/Vulkan/UnorderedVertexBufferView.h>
#include <GHA/Vulkan/IndexBufferView.h>
#include <GHA/Vulkan/TextureBufferView.h>
#include <GHA/Vulkan/UnorderedTextureBufferView.h>
#include <GHA/Vulkan/UnorderedIndexBufferView.h>
#include <GHA/Vulkan/StructuredBufferView.h>
#include <GHA/Vulkan/UnorderedStructuredBufferView.h>
#include <GHA/Vulkan/EnumUtils.h>

DJINN_NS3(GHA, Vulkan);

DescriptorSet::DescriptorSet(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUnknown(registry, outer, host) {

}

COM::Result DescriptorSet::Initialize(const DescriptorSetUpdateDesc &desc, DescriptorSetLayout *layout, DescriptorAllocator *allocator, vk::Device device) {
    this->device = device;
    this->desc = desc;
    this->allocator = allocator;

    // Create allocation
    if (!allocator->Allocate((layout)->GetLayout(), static_cast<UInt32>(desc.bindings.Size()), desc.bindings.Ptr(), &alloc)) {
        ErrorLog(this).Write("Vulkan descriptor set allocation failed");
        return COM::kOK;
    }

    // ...
    set = alloc->set;

    // Update
    Check(Update(desc));

    // OK
    return COM::kOK;
}


const DescriptorSetUpdateDesc &DescriptorSet::GetDesc() {
    return desc;
}

COM::Result DescriptorSet::Update(const DescriptorSetUpdateDesc &desc) {
    // Final writes
    vk::WriteDescriptorSet writes[128];
    vk::DescriptorImageInfo imageDescriptors[128];

    // Translate
    for (UInt32 i = 0; i < desc.bindings.Size(); i++) {
        writes[i].dstSet = set;
        writes[i].descriptorCount = 1;
        writes[i].descriptorType = AsDescriptorType(desc.bindings[i].resource->GetClassID());
        writes[i].dstArrayElement = desc.bindings[i].arrayElement;
        writes[i].dstBinding = desc.bindings[i].binding.index;

        switch (desc.bindings[i].resource->GetClassID()) {
            case TextureView::kCID:
                imageDescriptors[i] = static_cast<TextureView *>(desc.bindings[i].resource)->GetDescriptor();
                imageDescriptors[i].sampler = static_cast<Sampler *>(desc.bindings[i].sampler)->GetSampler();
                writes[i].pImageInfo = &imageDescriptors[i];
                break;
            case ConstantBuffer::kCID:
                writes[i].pBufferInfo = &static_cast<ConstantBuffer *>(desc.bindings[i].resource)->GetDescriptor();
                break;
            case UnorderedTextureView::kCID:
                writes[i].pImageInfo = &static_cast<UnorderedTextureView *>( desc.bindings[i].resource)->GetDescriptor();
                break;
            case UnorderedVertexBufferView::kCID:
                writes[i].pBufferInfo = &static_cast<UnorderedVertexBufferView *>( desc.bindings[i].resource)->GetDescriptor();
                break;
            case IndexBufferView::kCID:
                writes[i].pTexelBufferView = &static_cast<IndexBufferView *>( desc.bindings[i].resource)->GetView();
                break;
            case UnorderedIndexBufferView::kCID:
                writes[i].pTexelBufferView = &static_cast<UnorderedIndexBufferView *>( desc.bindings[i].resource)->GetView();
                break;
            case StructuredBufferView::kCID:
                writes[i].pBufferInfo = &static_cast<StructuredBufferView *>( desc.bindings[i].resource)->GetDescriptor();
                break;
            case UnorderedStructuredBufferView::kCID:
                writes[i].pBufferInfo = &static_cast<UnorderedStructuredBufferView *>( desc.bindings[i].resource)->GetDescriptor();
                break;
            case TextureBufferView::kCID:
                writes[i].pTexelBufferView = &static_cast<TextureBufferView *>( desc.bindings[i].resource)->GetView();
                break;
            case UnorderedTextureBufferView::kCID:
                writes[i].pTexelBufferView = &static_cast<UnorderedTextureBufferView *>( desc.bindings[i].resource)->GetView();
                break;
            default:
                ErrorLog(this).Write("Vulkan descriptor set update failed, invalid binding");
                return COM::kOK;
        }
    }

    // Update
    device.updateDescriptorSets(static_cast<UInt32>(desc.bindings.Size()), writes, 0, nullptr);

    // OK
    return COM::kOK;
}

COM::Result DescriptorSet::Released() {
    allocator->Free(alloc);

    // OK
    return COM::kOK;
}
