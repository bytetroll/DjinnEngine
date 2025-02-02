#include <GHA/Vulkan/TextureBuffer.h>
#include <GHA/Vulkan/Capabilities.h>
#include <GHA/Vulkan/Allocator.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <GHA/Vulkan/EnumUtils.h>

DJINN_NS3(GHA, Vulkan);

TextureBuffer::TextureBuffer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ITextureBuffer(registry, outer, host) {

}

COM::Result TextureBuffer::Initialize(const GHA::TextureBufferDesc &desc, GHA::ICapabilities *, vk::Device device, Allocator *allocator) {
    this->desc = desc;
    this->device = device;
    this->allocator = allocator;

    // Attempt to create buffer
    vk::BufferCreateInfo info;
    info.flags = {};
    info.size = desc.stride * desc.elementCount;
    info.sharingMode = vk::SharingMode::eExclusive;
    info.usage = vk::BufferUsageFlagBits::eStorageTexelBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc;
    if (vk::CheckedResult result = device.createBuffer(&info, nullptr, &buffer)) {
        ErrorLog(this).Write("Failed to create vulkan storage texel buffer: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Get memory requirements
    device.getBufferMemoryRequirements(buffer, &requirements);

    // Allocate memory
    if (!allocator->Allocate(requirements, vk::MemoryPropertyFlagBits::eDeviceLocal, "tbuffer"_id, &alloc)) {
        ErrorLog(this).Write("Failed to allocate memory for storage texel buffer");
        return COM::kError;
    }

    // Bind to buffer
    device.bindBufferMemory(buffer, alloc->deviceMemory, alloc->offset);

    // OK
    return COM::kOK;
}


const GHA::TextureBufferDesc &TextureBuffer::GetDesc() {
    return desc;
}

USize TextureBuffer::GetSize() {
    return requirements.size;
}

COM::Result TextureBuffer::Map(void **out, const Core::Optional<MemoryBounds> &bounds) {
    auto begin = alloc->offset + bounds->beginOffset.width;
    if (vk::CheckedResult result = device.mapMemory(alloc->deviceMemory, begin, bounds.IsValid() ? (bounds->endOffset.width - begin) : alloc->size, {}, out)) {
        ErrorLog(this).Write("Failed to map vulkan texture buffer: ", vk::to_string(result).c_str());
        return COM::kError;
    }
    return COM::kOK;
}

COM::Result TextureBuffer::Unmap(void *ptr) {
    device.unmapMemory(alloc->deviceMemory);
    return COM::kOK;
}

COM::Result TextureBuffer::Released() {
    device.destroyBuffer(buffer, nullptr);
    allocator->Free(alloc);

    // OK
    return COM::kOK;
}
