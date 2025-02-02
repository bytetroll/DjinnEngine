//
// Created by Dev on 11/12/2018.
//

#include <UI/ImmediateBuilder.h>
#include <Graphics/UIBrushPacket.h>
#include <Graphics/UIDrawPacket.h>
#include <Data/IFontAsset.h>
#include <Core/Alloca.h>
#include <UI/ElementComponents/IUITransformComponent.h>

DJINN_NS2(UI);

ImmediateBuilder::ImmediateBuilder(IUITransformComponent *com, Graphics::UIDrawPacketBuffer *buffer) : buffer(buffer) {
    viewport = com->GetViewport();
    transform = com->GetPixelTransform();
}

void ImmediateBuilder::SetViewport(const Math::Vector4 &viewport) {
    this->absoluteViewport = this->viewport = viewport;
}

void ImmediateBuilder::SetLocalViewport(const Math::Vector2 &offset, const Math::Vector2 &size) {
    viewport = {
            Math::Max(transform.w.x + offset.x, absoluteViewport.x),
            Math::Max(transform.w.y + offset.y, absoluteViewport.y),
            Math::Min(transform.w.x + offset.x + size.x, absoluteViewport.z),
            Math::Min(transform.w.y + offset.y + size.y, absoluteViewport.w)
    };
}

void ImmediateBuilder::SetTransform(const Math::Matrix4 &transform) {
    this->transform = transform;
}

void ImmediateBuilder::SetBrush(const Graphics::UIBrushPacket &packet) {
    this->brush = packet;
}

void ImmediateBuilder::SetFont(const Data::AssetView<Data::IFontAsset> &font) {
    this->font = font;
}

void ImmediateBuilder::SetPositionMode(ImmediatePositionMode mode, const Math::Vector2 &container) {
    this->mode = mode;
    this->positionContainer = container;
}

void ImmediateBuilder::Box(const Math::Vector2 &offset, const Math::Vector2 &size) {
    Graphics::UIDrawPacket packet;
    packet.brush = brush;
    packet.viewport = viewport;
    packet.transform = Math::Matrix4::Translate(transform, {GetOffset(offset, size), 0});
    packet.transform.w.z += depthOffset;
    packet.geometry = Graphics::UIGeometryBox(size);
    buffer->Add(packet);
}

void ImmediateBuilder::MarginBox(const Math::Vector2 &offset, const Math::Vector2 &size, const Math::Vector2 &margin) {
    Graphics::UIDrawPacket packet;
    packet.brush = brush;
    packet.viewport = viewport;
    packet.transform = Math::Matrix4::Translate(transform, {GetOffset(offset, size), 0});
    packet.transform.w.z += depthOffset;
    packet.geometry = Graphics::UIGeometryMarginBox(size, margin);
    buffer->Add(packet);
}

void ImmediateBuilder::Line(const Graphics::UIGeometryVertex &start, const Graphics::UIGeometryVertex &end, float width) {
    Graphics::UIGeometryVertex vertices[] = {start, end};
    Lines(vertices, width, false);
}

void ImmediateBuilder::Lines(const Core::ArrayView<Graphics::UIGeometryVertex> &vertices, float width, bool connected) {
    Graphics::UIDrawPacket packet;
    packet.brush = brush;
    packet.viewport = viewport;
    packet.transform = transform;
    packet.transform.w.z += depthOffset;
    packet.geometry = Graphics::UIGeometryLine(vertices, false, width);
    buffer->Add(packet);
}

void ImmediateBuilder::Label(IntermediateCache &cache, const Math::Vector2 &offset, const Core::StringView &str, const Math::Vector4 &color, float smoothing) {
    DJINN_ASSERT(font.Get(), "Cannot render without a font attached");

    // ...
    cache.glyphRenders.Resize(str.Length());

    // Prepare request
    Data::FontRenderRequest request({cache.glyphRenders});

    // Attempt to render
    Data::FontRenderConfig config;
    if (!font->RenderGlyphs(str, config, request)) {
        return;
    }

    Graphics::UIDrawPacket packet;
    packet.brush = Graphics::UIBrushSDF(color, smoothing);
    packet.transform = Math::Matrix4::Translate(transform, {GetOffset(offset, request.offset + Math::Vector2(0, config.size + config.lineSeperation)), 0});
    packet.transform.w.z += depthOffset;
    packet.geometry = Graphics::UIGeometryFont(request.glyphs.GetView(), font);
    packet.viewport = viewport;
    buffer->Add(packet);
}

Math::Vector2 ImmediateBuilder::GetOffset(const Math::Vector2 &offset, const Math::Vector2 &size) {
    switch (mode) {
        case ImmediatePositionMode::eNone: {
            return offset;
        }
        case ImmediatePositionMode::eCenter: {
            return positionContainer * .5f - size * .5f + offset;
        }
    }

    return offset;
}

void ImmediateBuilder::SetDepthOffset(int offset) {
    this->depthOffset = offset;
}

void ImmediateBuilder::ResetLocalViewport() {
    viewport = absoluteViewport;
}
