#include <UI/Elements/TransformDebug.h>
#include <Graphics/UIDrawPacket.h>
#include <UI/IUICanvas.h>
#include <UI/ElementComponents/IUITransformComponent.h>
#include <UI/ElementComponents/UILeafTransformComponent.h>

DJINN_NS2(UI);

TransformDebug::TransformDebug(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void TransformDebug::StaticInitialize() {
    SetTransform(CreateComponent<UILeafTransformComponent>());

    CreateComponent<Renderer>();
}

TransformDebug::Renderer::Renderer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIRenderComponent(registry, outer, host) {

}

void TransformDebug::Renderer::Build(Graphics::UIDrawPacketBuffer *buffer) {
    /*for (auto com : GetElement()->GetCanvas()->GetRenderComponents()) {
        auto element = com->GetElement();

        auto viewport = element->GetTransform()->GetViewport();

        {
            Graphics::UIDrawPacket packet;
            packet.transform = Math::Matrix4::Translate({viewport.x, viewport.y, 1000});
            packet.brush = Graphics::UIBrushSolid({0, 1, 0, 0.15f});
            packet.geometry = Graphics::UIGeometryBox({viewport.z - viewport.x, viewport.w - viewport.y});
            packet.viewport = {0, 0, 1e6, 1e6};
            buffer->Add(packet);
        }

        {
            Graphics::UIDrawPacket packet;
            packet.transform = element->GetTransform()->GetPixelTransform();
            packet.transform.w.z += 1000;
            packet.brush = Graphics::UIBrushSolid({1, 0, 0, 0.35f});
            packet.geometry = Graphics::UIGeometryBox({element->GetTransform()->GetDesiredSize().x, element->GetTransform()->GetDesiredSize().y});
            packet.viewport = {0, 0, 1e6, 1e6};
            buffer->Add(packet);
        }
    }*/
}
