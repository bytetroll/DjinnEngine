//
// Created by Dev on 12/9/2017.
//

#include <GHA/Vulkan/StructuredBufferView.h>
#include <GHA/Vulkan/StructuredBuffer.h>

DJINN_NS3(GHA, Vulkan);

StructuredBufferView::StructuredBufferView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IResource(registry, outer, host) {

}

COM::Result StructuredBufferView::Initialize(vk::Device, IStructuredBuffer *structuredBuffer) {
    this->structuredBuffer = structuredBuffer;

    // Describe descriptor
    descriptor.offset = 0;
    descriptor.range = structuredBuffer->GetSize();
    descriptor.buffer = static_cast<StructuredBuffer*>(structuredBuffer)->GetBuffer();

    // OK
    return COM::kOK;
}

IStructuredBuffer *StructuredBufferView::GetStructuredBuffer() {
    return structuredBuffer;
}

COM::Result StructuredBufferView::Released() {
    // OK
    return COM::kOK;
}
