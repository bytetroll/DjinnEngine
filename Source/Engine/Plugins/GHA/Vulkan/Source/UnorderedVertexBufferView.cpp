//
// Created by Dev on 12/9/2017.
//

#include <GHA/Vulkan/UnorderedVertexBufferView.h>
#include <GHA/Vulkan/VertexBuffer.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>

DJINN_NS3(GHA, Vulkan);

UnorderedVertexBufferView::UnorderedVertexBufferView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IResource(registry, outer, host) {

}

COM::Result UnorderedVertexBufferView::Initialize(vk::Device, IVertexBuffer* vertexBuffer) {
    this->vertexBuffer = vertexBuffer;
    auto vkVertexBuffer = static_cast<VertexBuffer*>(vertexBuffer);

    // Describe descriptor
    descriptor.offset = 0;
    descriptor.range = vkVertexBuffer->GetSize();
    descriptor.buffer = vkVertexBuffer->GetBuffer();

    // OK
    return COM::kOK;
}

IVertexBuffer *UnorderedVertexBufferView::GetVertexBuffer() {
    return vertexBuffer;
}

COM::InterfaceID UnorderedVertexBufferView::GetIID() {
    return kIID;
}

COM::Result UnorderedVertexBufferView::Released() {
    // OK
    return COM::kOK;
}
