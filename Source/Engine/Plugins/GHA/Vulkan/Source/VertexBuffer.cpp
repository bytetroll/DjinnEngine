#include <GHA/Vulkan/VertexBuffer.h>
#include <GHA/Vulkan/Allocator.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>

DJINN_NS3(GHA, Vulkan);

VertexBuffer::VertexBuffer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IVertexBuffer(registry, outer, host) {

}

COM::Result VertexBuffer::Initialize(const GHA::VertexBufferDesc &desc, vk::Device device, Allocator *allocator) {
    this->desc = desc;
    this->device = device;
    this->allocator = allocator;

    // Attempt to create buffer
    vk::BufferCreateInfo info;
    info.flags = {};
    info.size = desc.stride * desc.count;
    info.sharingMode = vk::SharingMode::eExclusive;
    info.usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc;
    if (vk::CheckedResult result = device.createBuffer(&info, nullptr, &buffer)) {
        ErrorLog(this).Write("Failed to create vulkan vertex buffer: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Get memory requirements
    device.getBufferMemoryRequirements(buffer, &requirements);

    // Allocate memory
    if (!allocator->Allocate(requirements, desc.hostVisible ? vk::MemoryPropertyFlagBits::eHostVisible : vk::MemoryPropertyFlagBits::eDeviceLocal, "vbuffer"_id, &alloc)) {
        ErrorLog(this).Write("Failed to allocate memory for vertex buffer");
        return COM::kError;
    }

    // Bind to buffer
    if (vk::CheckedResult result = device.bindBufferMemory(buffer, alloc->deviceMemory, alloc->offset)) {
        ErrorLog(this).Write("Failed to bind vulkan allocation to buffer: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // OK
    return COM::kOK;
}


const GHA::VertexBufferDesc &VertexBuffer::GetDesc() {
    return desc;
}

USize VertexBuffer::GetSize() {
    return requirements.size;
}

COM::Result VertexBuffer::Map(void **out, const Core::Optional<MemoryBounds> &bounds) {
    auto begin = alloc->offset + bounds->beginOffset.width;
    if (vk::CheckedResult result = device.mapMemory(alloc->deviceMemory, begin, bounds.IsValid() ? (bounds->endOffset.width - begin) : alloc->size, {}, out)) {
        ErrorLog(this).Write("Failed to map vulkan vertex buffer: ", vk::to_string(result).c_str());
        return COM::kError;
    }
    return COM::kOK;
}

COM::Result VertexBuffer::Unmap(void *ptr) {
    device.unmapMemory(alloc->deviceMemory);
    return COM::kOK;
}

COM::Result VertexBuffer::Released() {
    device.destroyBuffer(buffer, nullptr);
    allocator->Free(alloc);

    // OK
    return COM::kOK;
}
