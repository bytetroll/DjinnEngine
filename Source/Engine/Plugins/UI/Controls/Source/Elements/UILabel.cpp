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

#include <UI/Elements/UILabel.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <Graphics/UIDrawPacket.h>
#include <Data/IFontAsset.h>
#include <UI/ElementComponents/UILeafTransformComponent.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(UI);

UILabel::UILabel(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : IUIElement(registry, outer, host) {

}

void UILabel::StaticInitialize() {
    // Default transform
    SetTransform(CreateComponent<UILeafTransformComponent>());

    // Renderer
    renderer = CreateComponent<Renderer>();
    {
        renderer->text = "Label";
    }

    // Get asset worldHost
    Data::IAssetHost *host;
    if (!registry->GetPipeline()->GetInterface(&host)) {
        ErrorLog(this).Write("Expected an asset worldHost to be installed");
        return;
    }

    // Default font
    renderer->font = host->CompileAssetAsync<Data::IFontAsset>("Runtime/Engine/Fonts/Akrobat-Black.otf"_path);

    // ...
    MarkAsUncached();
}

bool UILabel::Cache() {
    // Must have font attached
    if (!renderer->font.IsValid()) {
        return true;
    }

    // Ready?
    Data::IFontAsset *asset;
    if (!(asset = renderer->font.GetRaw())) {
        return false;
    }

    // Ensure size
    renderer->glyphRenders.Resize(renderer->text.Length());

    // Prepare request
    Data::FontRenderRequest request({renderer->glyphRenders});

    // Render glyphs
    if (!asset->RenderGlyphs(
            renderer->text,
            Data::FontRenderConfig()
                    .Size(renderer->fontSize)
                    .LineSeperation(renderer->lineSeperation),
            request
    )) {
        ErrorLog(this).Write("Failed to render font glyphs with text: '", renderer->text, "'");
        return true;
    }

    // Some glyphs may be ignored
    renderer->glyphRenders.Resize(request.glyphs.GetSize());

    // Set transform
    GetTransform()->SetDesiredSize(request.offset + Math::Vector2(0, renderer->fontSize + renderer->lineSeperation));

    // OK
    return true;
}

void UILabel::StyleEvent(const ElementStyle &style) {
    renderer->color = style.GetOrDefault("color"_id, renderer->color);
}

UILabel::Renderer::Renderer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : IUIRenderComponent(registry, outer, host) {

}

void UILabel::Renderer::Build(Graphics::UIDrawPacketBuffer *buffer) {
    auto transform = GetElement()->GetTransform();

    // Ignore no glyphs
    if (glyphRenders.Size() == 0 || !font) {
        return;
    }

    // Submit packet
    Graphics::UIDrawPacket packet;
    packet.transform = transform->GetPixelTransform();
    packet.brush = Graphics::UIBrushSDF(color, 1.0f / smoothing);
    Graphics::UIGeometryFont geometryFont;
    geometryFont.glyphs = glyphRenders;
    geometryFont.font = font;
    packet.geometry = geometryFont;
    packet.viewport = transform->GetViewport();
    buffer->Add(packet);
}
