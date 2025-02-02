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
 on 12/9/2017.
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
