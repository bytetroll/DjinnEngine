//
// Created by Dev on 12/9/2017.
//

#include <GHA/Vulkan/UnorderedTextureBufferView.h>
#include <GHA/Vulkan/TextureBufferView.h>

DJINN_NS3(GHA, Vulkan);

UnorderedTextureBufferView::UnorderedTextureBufferView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IResource(registry, outer, host) {

}

COM::Result UnorderedTextureBufferView::Initialize(vk::Device, TextureBufferView* textureBufferView) {
    this->textureBufferView = textureBufferView;
    this->view = (textureBufferView)->GetView();

    // OK
    return COM::kOK;
}

TextureBufferView *UnorderedTextureBufferView::GetTextureBufferView() {
    return textureBufferView;
}

COM::InterfaceID UnorderedTextureBufferView::GetIID() {
    return kIID;
}

COM::Result UnorderedTextureBufferView::Released() {
    // OK
    return COM::kOK;
}
