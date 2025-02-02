#include <GHA/Vulkan/StructuredBuffer.h>
#include <GHA/Vulkan/Allocator.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <GHA/Vulkan/EnumUtils.h>

DJINN_NS3(GHA, Vulkan);

StructuredBuffer::StructuredBuffer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IStructuredBuffer(registry, outer, host) {

}

COM::Result StructuredBuffer::Initialize(const GHA::StructuredBufferDesc &desc, vk::Device device, Allocator *allocator) {
    this->desc = desc;
    this->device = device;
    this->allocator = allocator;

    // Attempt to create buffer
    vk::BufferCreateInfo info;
    info.flags = {};
    info.size = desc.size.width;
    info.sharingMode = vk::SharingMode::eExclusive;
    info.usage = vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc;
    if (vk::CheckedResult result = device.createBuffer(&info, nullptr, &buffer)) {
        ErrorLog(this).Write("Failed to create vulkan structured buffer: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Get memory requirements
    device.getBufferMemoryRequirements(buffer, &requirements);

    // Allocate memory
    if (!allocator->Allocate(requirements, vk::MemoryPropertyFlagBits::eDeviceLocal, "cbuffer"_id, &alloc)) {
        ErrorLog(this).Write("Failed to allocate memory for structured buffer");
        return COM::kError;
    }

    // Bind to buffer
    device.bindBufferMemory(buffer, alloc->deviceMemory, alloc->offset);

    // Create view
    vk::BufferViewCreateInfo viewInfo;
    viewInfo.buffer = buffer;
    viewInfo.offset = 0;
    viewInfo.range = requirements.size;
    viewInfo.format = vk::Format::eUndefined;
    if (vk::CheckedResult result = device.createBufferView(&viewInfo, nullptr, &view)) {
        ErrorLog(this).Write("Failed to create vulkan storage buffer view: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // OK
    return COM::kOK;
}


const GHA::StructuredBufferDesc &StructuredBuffer::GetDesc() {
    return desc;
}

USize StructuredBuffer::GetSize() {
    return requirements.size;
}

COM::Result StructuredBuffer::Map(void **out, const Core::Optional<MemoryBounds> &bounds) {
    auto begin = alloc->offset + bounds->beginOffset.width;
    if (vk::CheckedResult result = device.mapMemory(alloc->deviceMemory, begin, bounds.IsValid() ? (bounds->endOffset.width - begin) : alloc->size, {}, out)) {
        ErrorLog(this).Write("Failed to map vulkan structured buffer: ", vk::to_string(result).c_str());
        return COM::kError;
    }
    return COM::kOK;
}

COM::Result StructuredBuffer::Unmap(void *ptr) {
    device.unmapMemory(alloc->deviceMemory);
    return COM::kOK;
}

COM::Result StructuredBuffer::Released() {
    device.destroyBufferView(view, nullptr);
    allocator->Free(alloc);

    // OK
    return COM::kOK;
}
