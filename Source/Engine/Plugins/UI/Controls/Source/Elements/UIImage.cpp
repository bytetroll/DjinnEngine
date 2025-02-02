//
// Created by Dev on 8/23/2018.
//

#include <UI/Elements/UIImage.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <Graphics/UIDrawPacket.h>
#include <Data/ITextureAsset.h>
#include <UI/ElementComponents/UILeafTransformComponent.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(UI);

UIImage::UIImage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UIImage::StaticInitialize() {
    // Default transform
    SetTransform(CreateComponent<UILeafTransformComponent>());

    // Renderer
    renderer = CreateComponent<Renderer>();

    // Get asset worldHost
    if (!registry->GetPipeline()->GetInterface(&host)) {
        ErrorLog(this).Write("Expected an asset worldHost to be installed");
        return;
    }

    // Default image
    renderer->texture = host->CompileAssetAsync<Data::ITextureAsset>("Runtime/Engine/UI/Missing.png"_path);
}

void UIImage::LoadTexture(const Core::StringView &path) {
    renderer->texture = host->CompileAssetAsync<Data::ITextureAsset>(Base::ToPath(path.Ptr()));
}

void UIImage::StyleEvent(const ElementStyle &style) {
    if (auto margin = style.Get("margin-geometry"_id)) {
        SetMarginGeometry(margin->Get<Math::Vector2>());
    }
}

UIImage::Renderer::Renderer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIRenderComponent(registry, outer, host) {

}

void UIImage::Renderer::Build(Graphics::UIDrawPacketBuffer *buffer) {
    auto transform = GetElement()->GetTransform();

    Graphics::UIDrawPacket packet;
    packet.transform = transform->GetPixelTransform();
    packet.viewport = transform->GetViewport();
    if (texture) {
        packet.brush = Graphics::UIBrushTexture(color, texture, GHA::Samplers::noMip);
    } else {
        packet.brush = Graphics::UIBrushSolid(color);
    }

    switch (geometry.type) {
        default:
            break;
        case Graphics::UIGeometryType::eBox:
            geometry.box.size = transform->GetDesiredSize();
            break;
        case Graphics::UIGeometryType::eMarginBox:
            geometry.marginBox.size = transform->GetDesiredSize();
            break;
        case Graphics::UIGeometryType::eCircle:
            geometry.circle.radii = transform->GetDesiredSize() / 2.0f;
            break;
    }
    packet.geometry = geometry;
    buffer->Add(packet);
}
