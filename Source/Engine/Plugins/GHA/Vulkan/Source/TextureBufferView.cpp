//
// Created by Dev on 12/9/2017.
//

#include <GHA/Vulkan/TextureBufferView.h>
#include <GHA/Vulkan/TextureBuffer.h>
#include <GHA/Vulkan/EnumUtils.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>

DJINN_NS3(GHA, Vulkan);

TextureBufferView::TextureBufferView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IResource(registry, outer, host) {

}

COM::Result TextureBufferView::Initialize(const TextureBufferViewDesc &desc, vk::Device device, ITextureBuffer *textureBuffer) {
    this->textureBuffer = textureBuffer;
    this->desc = desc;
    this->device = device;

    auto vkTextureBuffer = static_cast<TextureBuffer *>(textureBuffer);

    // Create view
    vk::BufferViewCreateInfo viewInfo;
    viewInfo.buffer = vkTextureBuffer->GetBuffer();
    viewInfo.offset = 0;
    viewInfo.range = vkTextureBuffer->GetSize();
    viewInfo.format = AsFormat(desc.format);
    if (vk::CheckedResult result = device.createBufferView(&viewInfo, nullptr, &view)) {
        ErrorLog(this).Write("Failed to create vulkan storage texel buffer view: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // OK
    return COM::kOK;
}


ITextureBuffer *TextureBufferView::GetTextureBuffer() {
    return textureBuffer;
}

const TextureBufferViewDesc &TextureBufferView::GetDesc() {
    return desc;
}

COM::Result TextureBufferView::Released() {
    device.destroyBufferView(view, nullptr);

    // OK
    return COM::kOK;
}
