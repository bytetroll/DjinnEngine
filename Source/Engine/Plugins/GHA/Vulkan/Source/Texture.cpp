#include <GHA/Vulkan/Texture.h>
#include <GHA/Vulkan/Allocator.h>
#include <GHA/Vulkan/EnumUtils.h>
#include <Base/DefaultLogs.h>
#include <GHA/ICapabilities.h>
#include <Host/IRegistry.h>
#include <GHA/Vulkan/Capabilities.h>

DJINN_NS3(GHA, Vulkan);

Texture::Texture(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : PipelineScheduler::ITexture(registry, outer, host) {

}

COM::Result Texture::Initialize(const GHA::TextureDesc &desc, vk::Device device, Allocator *allocator, ICapabilities *capabilities) {
    this->desc = desc;
    this->device = device;
    this->allocator = allocator;
    this->formatSize = capabilities->GetFormatSize(desc.format);

    // Attempt to create image
    vk::ImageCreateInfo info;
    info.flags = {};
    info.imageType = AsImageType(desc.type);
    info.usage = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eTransferSrc;
    info.samples = AsSampleCountFlag(desc.samples);
    info.format = AsFormat(desc.format);
    info.extent = vk::Extent3D(desc.size.width, desc.size.height, desc.size.depth);
    info.mipLevels = desc.mipLevels ?: (Math::Floor(Math::Log2(Math::Max(desc.size.width, desc.size.height, desc.size.depth))) + 1);
    info.arrayLayers = desc.arrayCount.IsValid() ? desc.arrayCount.Get() : 1;
    info.tiling = vk::ImageTiling::eOptimal;
    info.sharingMode = vk::SharingMode::eExclusive;
    info.initialLayout = vk::ImageLayout::eUndefined;

    // Attachment support?
    if (static_cast<Capabilities *>(capabilities)->IsFormatSupported(desc.format, GHA::FormatCapability::eRenderTarget) || (desc.usage & TextureUsage::eDepthStencil)) {
        info.usage |= AsImageUsageFlags(desc.usage);
    }

    // Create image
    if (vk::CheckedResult result = device.createImage(&info, nullptr, &image)) {
        ErrorLog(this).Write("Failed to create vulkan image: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // ...
    //    this->initializationCount = info.mipLevels;
    this->desc.mipLevels = info.mipLevels;

    // Get memory requirements
    device.getImageMemoryRequirements(image, &requirements);

    // Allocate memory
    if (!allocator->Allocate(requirements, vk::MemoryPropertyFlagBits::eDeviceLocal, "texture"_id, &alloc)) {
        ErrorLog(this).Write("Failed to allocate memory for image");
        return COM::kError;
    }

    // Bind to buffer
    device.bindImageMemory(image, alloc->deviceMemory, alloc->offset);

    // OK
    return COM::kOK;
}

COM::Result Texture::Initialize(const TextureDesc &desc, vk::Image image, vk::Device device) {
    this->desc = desc;
    this->image = image;
    this->alloc = nullptr;

    // Get memory requirements
    device.getImageMemoryRequirements(image, &requirements);

    // OK
    return COM::kOK;
}


const GHA::TextureDesc &Texture::GetDesc() {
    return desc;
}

USize Texture::GetSize() {
    return requirements.size;
}

COM::Result Texture::Map(void **out, const Core::Optional<MemoryBounds> &bounds) {
    auto begin = alloc->offset + bounds->beginOffset.width;
    if (vk::CheckedResult result = device.mapMemory(alloc->deviceMemory, begin, bounds.IsValid() ? (bounds->endOffset.width - begin) : alloc->size, {}, out)) {
        ErrorLog(this).Write("Failed to map vulkan texture: ", vk::to_string(result).c_str());
        return COM::kError;
    }
    return COM::kOK;
}

COM::Result Texture::Unmap(void *ptr) {
    device.unmapMemory(alloc->deviceMemory);
    return COM::kOK;
}

COM::Result Texture::Released() {
    if (device) {
        device.destroyImage(image, nullptr);
    }
    if (allocator) {
        allocator->Free(alloc);
    }
    return COM::kOK;
}
