//
// Created by Dev on 12/9/2017.
//

#include <GHA/Vulkan/UnorderedStructuredBufferView.h>
#include <GHA/Vulkan/StructuredBufferView.h>

DJINN_NS3(GHA, Vulkan);

UnorderedStructuredBufferView::UnorderedStructuredBufferView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IResource(registry, outer, host) {

}

COM::Result UnorderedStructuredBufferView::Initialize(vk::Device, StructuredBufferView* structuredBufferView) {
    this->structuredBufferView = structuredBufferView;
    this->descriptor = (structuredBufferView)->GetDescriptor();

    // OK
    return COM::kOK;
}

StructuredBufferView *UnorderedStructuredBufferView::GetStructuredBufferView() {
    return structuredBufferView;
}

COM::Result UnorderedStructuredBufferView::Released() {
    // OK
    return COM::kOK;
}
