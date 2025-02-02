//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 8/23/2018.
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
