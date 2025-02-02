#include <GHA/Vulkan/TextureView.h>
#include <GHA/Vulkan/EnumUtils.h>
#include <GHA/Vulkan/Texture.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>

DJINN_NS3(GHA, Vulkan);

TextureView::TextureView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IResource(registry, outer, host) {

}

COM::Result TextureView::Initialize(const GHA::TextureViewDesc &desc, vk::Device device, GHA::ITexture *texture) {
    this->texture = texture;
    this->desc = desc;
    this->device = device;

    auto textureDesc = texture->GetDesc();

    if (desc.viewType == TextureViewType::eUndefined) {
        switch (textureDesc.usage.Get()) {
            case TextureUsage::eColor:
                this->desc.viewType = TextureViewType::eColor;
                break;
            case TextureUsage::eDepthStencil:
                this->desc.viewType = TextureViewType::eDepth;
                break;
        }
    }

    // Describe view
    vk::ImageViewCreateInfo info;
    info.format = AsFormat(desc.format == GHA::Format::eUndefined ? textureDesc.format : desc.format);
    info.image = static_cast<Texture *>(texture)->GetImage();
    info.subresourceRange = vk::ImageSubresourceRange(
            AsImageAspectFlag(this->desc.viewType),
            this->desc.mipBounds->lower = static_cast<UInt32>(desc.mipBounds.IsValid() ? desc.mipBounds->lower : 0),
            this->desc.mipBounds->upper = static_cast<UInt32>(desc.mipBounds.IsValid() ? (desc.mipBounds->upper - desc.mipBounds->lower) : textureDesc.mipLevels),
            this->desc.arrayBounds->lower = desc.arrayBounds.IsValid() ? (desc.arrayBounds->lower) : 0,
            this->desc.arrayBounds->upper = desc.arrayBounds.IsValid() ? (desc.arrayBounds->upper - desc.arrayBounds->lower) : textureDesc.size.depth
    );

    // Convert view type
    switch (textureDesc.type) {
        case GHA::TextureType::e1D:
            info.viewType = (textureDesc.arrayCount.IsValid() ? vk::ImageViewType::e1DArray : vk::ImageViewType::e1D);
            break;
        case GHA::TextureType::e2D:
            info.viewType = (textureDesc.arrayCount.IsValid() ? vk::ImageViewType::e2DArray : vk::ImageViewType::e2D);
            break;
        case GHA::TextureType::e3D:
            info.viewType = vk::ImageViewType::e3D;
            break;
        case GHA::TextureType::eCube:
            info.viewType = (textureDesc.arrayCount.IsValid() ? vk::ImageViewType::eCubeArray : vk::ImageViewType::eCube);
            break;
    }

    // Attempt to create image view
    if (vk::CheckedResult result = device.createImageView(&info, nullptr, &view)) {
        ErrorLog(this).Write("Failed to create vulkan image view: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Prepare descriptor
    descriptor.sampler = nullptr;
    descriptor.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
    descriptor.imageView = view;

    // OK
    return COM::kOK;
}

const GHA::TextureViewDesc &TextureView::GetDesc() {
    return desc;
}

GHA::ITexture *TextureView::GetTexture() {
    return texture;
}

COM::Result TextureView::Released() {
    device.destroyImageView(view, nullptr);

    // OK
    return COM::kOK;
}