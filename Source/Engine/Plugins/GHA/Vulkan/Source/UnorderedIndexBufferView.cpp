//
// Created by Dev on 12/9/2017.
//

#include <GHA/Vulkan/UnorderedIndexBufferView.h>
#include <GHA/Vulkan/IndexBufferView.h>

DJINN_NS3(GHA, Vulkan);

UnorderedIndexBufferView::UnorderedIndexBufferView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IResource(registry, outer, host) {

}

COM::Result UnorderedIndexBufferView::Initialize(vk::Device, IndexBufferView* indexBufferView) {
    this->indexBufferView = indexBufferView;
    this->view = (indexBufferView)->GetView();

    // OK
    return COM::kOK;
}


IndexBufferView *UnorderedIndexBufferView::GetIndexBufferView() {
    return indexBufferView;
}

COM::Result UnorderedIndexBufferView::Released() {
    // OK
    return COM::kOK;
}
