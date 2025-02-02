//
// Created by Dev on 8/23/2018.
//

#include <UI/Elements/UISpline.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <Graphics/UIDrawPacket.h>
#include <Data/IFontAsset.h>
#include <Base/DefaultLogs.h>
#include <UI/ElementComponents/UILeafTransformComponent.h>

DJINN_NS2(UI);

UISpline::UISpline(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UISpline::StaticInitialize() {
    // Default transform
    SetTransform(CreateComponent<UILeafTransformComponent>());

    // Renderer
    renderer = CreateComponent<Renderer>();
}

bool UISpline::Cache() {
    // Bounds
    Math::Vector2 min;
    Math::Vector2 max;

    // Calculate spline
    if (!renderer->spline.Compute()) {
        ErrorLog(this).Write("Failed to update ui spline component, invalid spline control points");
        return true;
    }

    // Template vertex
    Graphics::UIGeometryVertex vertex;

    // Generate vertices
    renderer->vertices.Clear();
    for (Size i = 0; i < static_cast<Size>(renderer->spline.controlPoints.Size()) - 1; i++) {
        for (UInt32 j = 0; j < renderer->subdivisions; j++) {
            vertex.position = renderer->spline.Evaluate(static_cast<UInt32>(i), static_cast<float>(j) / renderer->subdivisions);
            renderer->vertices.Add(vertex);

            min.x = Math::Min(min.x, vertex.position.x);
            min.y = Math::Min(min.y, vertex.position.y);

            max.x = Math::Max(max.x, vertex.position.x);
            max.y = Math::Max(max.y, vertex.position.y);
        }
    }

    // End
    vertex.position = renderer->spline.controlPoints.Last();
    renderer->vertices.Add(vertex);

    // Set transform
    GetTransform()->SetDesiredSize((max - min));

    return true;
}

UISpline::Renderer::Renderer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIRenderComponent(registry, outer, host) {

}

void UISpline::Renderer::Build(Graphics::UIDrawPacketBuffer *buffer) {
    auto transform = GetElement()->GetTransform();

    // Ignore no glyphs
    if (vertices.Size() == 0) {
        return;
    }

    // Submit packet
    Graphics::UIDrawPacket packet;
    packet.transform = transform->GetPixelTransform();
    packet.brush = Graphics::UIBrushSolid(color);
    Graphics::UIGeometryLine line;
    line.connected = true;
    line.vertices = vertices;
    line.width = width;
    packet.geometry = line;
    packet.viewport = transform->GetViewport();
    buffer->Add(packet);
}
