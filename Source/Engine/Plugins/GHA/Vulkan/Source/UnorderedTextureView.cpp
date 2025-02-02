//
// Created by Dev on 12/9/2017.
//

#include <GHA/Vulkan/UnorderedTextureView.h>
#include <GHA/Vulkan/TextureView.h>

DJINN_NS3(GHA, Vulkan);

UnorderedTextureView::UnorderedTextureView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IResource(registry, outer, host) {

}

COM::Result UnorderedTextureView::Initialize(vk::Device, TextureView* textureView) {
    this->textureView = textureView;
    this->descriptor = (textureView)->GetDescriptor();
    this->view = (textureView)->GetView();

    // OK
    return COM::kOK;
}

TextureView *UnorderedTextureView::GetTextureView() {
    return textureView;
}

COM::InterfaceID UnorderedTextureView::GetIID() {
    return kIID;
}

COM::Result UnorderedTextureView::Released() {
    // OK
    return COM::kOK;
}
