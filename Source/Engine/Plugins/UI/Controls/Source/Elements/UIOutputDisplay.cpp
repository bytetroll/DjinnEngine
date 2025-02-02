//
// Created by Dev on 8/23/2018.
//

#include <UI/Elements/UIOutputDisplay.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <Graphics/UIDrawPacket.h>
#include <UI/ElementComponents/UILeafTransformComponent.h>

DJINN_NS2(UI);

UIOutputDisplay::UIOutputDisplay(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UIOutputDisplay::StaticInitialize() {
    // Default transform
    SetTransform(CreateComponent<UILeafTransformComponent>());

    // Renderer
    renderer = CreateComponent<Renderer>();
}

UIOutputDisplay::Renderer::Renderer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIRenderComponent(registry, outer, host) {

}

void UIOutputDisplay::Renderer::Build(Graphics::UIDrawPacketBuffer *buffer) {
    auto transform = GetElement()->GetTransform();

    if (output) {
        Graphics::UIDrawPacket packet;
        packet.transform = transform->GetPixelTransform();
        packet.viewport = transform->GetViewport();
        packet.brush = Graphics::UIBrushOutputTexture(Math::Vector4(1, 1, 1, 1), output, GHA::Samplers::noMip);
        packet.geometry = Graphics::UIGeometryBox(transform->GetDesiredSize());
        buffer->Add(packet);
    } else {
        Graphics::UIDrawPacket packet;
        packet.transform = transform->GetPixelTransform();
        packet.viewport = transform->GetViewport();
        packet.brush = Graphics::UIBrushSolid(Math::Vector4(0, 0, 0, 1));
        packet.geometry = Graphics::UIGeometryBox(transform->GetDesiredSize());
        buffer->Add(packet);
    }
}
