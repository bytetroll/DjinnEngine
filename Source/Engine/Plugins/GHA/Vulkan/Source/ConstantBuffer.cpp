#include <GHA/Vulkan/ConstantBuffer.h>
#include <GHA/Vulkan/Allocator.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <Core/Literals.h>

DJINN_NS3(GHA, Vulkan);

ConstantBuffer::ConstantBuffer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IConstantBuffer(registry, outer, host) {

}

COM::Result ConstantBuffer::Initialize(const GHA::ConstantBufferDesc &desc, vk::Device device, Allocator *allocator) {
    this->desc = desc;
    this->allocator = allocator;
    this->device = device;

    // Attempt to create buffer
    vk::BufferCreateInfo info;
    info.flags = {};
    info.size = desc.size.width;
    info.sharingMode = vk::SharingMode::eExclusive;
    info.usage = vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc;
    if (vk::CheckedResult result = device.createBuffer(&info, nullptr, &buffer)) {
        ErrorLog(this).Write("Failed to create vulkan uniform buffer: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Get memory requirements
    device.getBufferMemoryRequirements(buffer, &requirements);

    // Allocate memory
    if (!allocator->Allocate(requirements, vk::MemoryPropertyFlagBits::eHostVisible, "cbuffer"_id, &alloc)) {
        ErrorLog(this).Write("Failed to allocate memory for constant buffer");
        return COM::kError;
    }

    // Bind to buffer
    device.bindBufferMemory(buffer, alloc->deviceMemory, alloc->offset);

    // Prepare descriptor
    descriptor.offset = 0;
    descriptor.range = desc.size.width;
    descriptor.buffer = buffer;

    // OK
    return COM::kOK;
}


const GHA::ConstantBufferDesc &ConstantBuffer::GetDesc() {
    return desc;
}

USize ConstantBuffer::GetSize() {
    return requirements.size;
}

COM::Result ConstantBuffer::Map(void **out, const Core::Optional<MemoryBounds> &bounds) {
    auto begin = alloc->offset + bounds->beginOffset.width;
    if (vk::CheckedResult result = device.mapMemory(alloc->deviceMemory, begin, bounds.IsValid() ? (bounds->endOffset.width - begin) : alloc->size, {}, out)) {
        ErrorLog(this).Write("Failed to map vulkan index buffer: ", vk::to_string(result).c_str());
        return COM::kError;
    }
    return COM::kOK;
}

COM::Result ConstantBuffer::Unmap(void *ptr) {
    device.unmapMemory(alloc->deviceMemory);
    return COM::kOK;
}

COM::Result ConstantBuffer::Released() {
    device.destroyBuffer(buffer, nullptr);
    allocator->Free(alloc);

    // OK
    return COM::kOK;
}
