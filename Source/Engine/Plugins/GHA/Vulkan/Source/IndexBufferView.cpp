//
// Created by Dev on 12/9/2017.
//

#include <GHA/Vulkan/IndexBufferView.h>
#include <GHA/Vulkan/IndexBuffer.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>

DJINN_NS3(GHA, Vulkan);

IndexBufferView::IndexBufferView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IResource(registry, outer, host) {

}

COM::Result IndexBufferView::Initialize(vk::Device device, IIndexBuffer *indexBuffer) {
    this->indexBuffer = indexBuffer;
    this->device = device;
    auto vkIndexBuffer = static_cast<IndexBuffer *>(indexBuffer);

    // Create view
    vk::BufferViewCreateInfo viewInfo;
    viewInfo.buffer = vkIndexBuffer->GetBuffer();
    viewInfo.offset = 0;
    viewInfo.range = vkIndexBuffer->GetSize();
    switch (indexBuffer->GetDesc().type) {
        case IndexType::eU16:
            viewInfo.format = vk::Format::eR16Uint;
            break;
        case IndexType::eU32:
            viewInfo.format = vk::Format::eR32Uint;
            break;
    }
    if (vk::CheckedResult result = device.createBufferView(&viewInfo, nullptr, &view)) {
        ErrorLog(this).Write("Failed to create vulkan storage texel buffer view: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // OK
    return COM::kOK;
}


IIndexBuffer *IndexBufferView::GetIndexBuffer() {
    return indexBuffer;
}

COM::InterfaceID IndexBufferView::GetIID() {
    return kIID;
}

COM::Result IndexBufferView::Released() {
    device.destroyBufferView(view, nullptr);

    // OK
    return COM::kOK;
}
