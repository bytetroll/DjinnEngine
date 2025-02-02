//
// Created by Dev on 8/23/2018.
//

#include <UI/Elements/UIGraph.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <Graphics/UIDrawPacket.h>
#include <Data/IFontAsset.h>
#include <Data/ITextureAsset.h>
#include <Base/DefaultLogs.h>
#include <UI/ElementComponents/UILeafTransformComponent.h>

DJINN_NS2(UI);

UIGraph::UIGraph(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UIGraph::StaticInitialize() {
    // Default transform
    SetTransform(CreateComponent<UILeafTransformComponent>());

    // Renderer
    renderer = CreateComponent<Renderer>();
}

bool UIGraph::Cache() {
    // Clear cached lines
    renderer->cachedLines.Clear();

    // Get desired size
    Math::Vector2 desiredSize = GetTransform()->GetDesiredSize();

    // Calculate X lines
    Graphics::UIGeometryVertex lineVertex;
    for (float x = 0; x < Math::Clamp(desiredSize.x / renderer->gridUnitSnap, 0, 200); x++) {
        lineVertex.position = {(x) * renderer->gridUnitSnap, 0};
        renderer->cachedLines.Add(lineVertex);
        lineVertex.position = {(x) * renderer->gridUnitSnap, desiredSize.y};
        renderer->cachedLines.Add(lineVertex);
    }
    lineVertex.position = {desiredSize.x, 0};
    renderer->cachedLines.Add(lineVertex);
    lineVertex.position = {desiredSize.x, desiredSize.y};
    renderer->cachedLines.Add(lineVertex);

    // Calculate Y lines
    for (float y = 0; y < Math::Clamp(desiredSize.y / renderer->gridUnitSnap, 0, 200); y++) {
        lineVertex.position = {0, ((y) * renderer->gridUnitSnap)};
        renderer->cachedLines.Add(lineVertex);
        lineVertex.position = {desiredSize.x, ((y) * renderer->gridUnitSnap)};
        renderer->cachedLines.Add(lineVertex);
    }
    lineVertex.position = {0, desiredSize.y};
    renderer->cachedLines.Add(lineVertex);
    lineVertex.position = {desiredSize.x, desiredSize.y};
    renderer->cachedLines.Add(lineVertex);

    // Determine viewport
    if (renderer->viewport.IsValid()) {
        renderer->cachedMax = renderer->viewport.Get();
    } else {
        renderer->cachedMax = {};
        for (UInt32 i = 0; i < renderer->plots.Size(); i++) {
            auto data = &renderer->plots[i]->data;
            for (USize j = 0; j < data->data.Size(); j++) {
                renderer->cachedMax.x = Math::Max(renderer->cachedMax.x, data->data.Get(j).x);
                renderer->cachedMax.y = Math::Max(renderer->cachedMax.y, data->data.Get(j).y);
            }
        }
    }

    // Calculate graphs
    for (UInt32 i = 0; i < renderer->plots.Size(); i++) {
        auto properties = renderer->plots[i]->properties;
        auto data = &renderer->plots[i]->data;

        // Must not be empty
        if (data->data.Size() == 0) {
            continue;
        }

        // Clear cached geometry
        renderer->plots[i]->cachedVertices.Clear();
        renderer->plots[i]->cachedIndices.Clear();

        // ...
        switch (properties.type) {
            case UIPlotType::eLine: {
                Graphics::UIGeometryVertex vertex;
                for (USize j = 0; j < data->data.Size() - 1; j++) {
                    // 1
                    vertex.position = {data->data.Get(j).x, renderer->cachedMax.y - Math::Min(renderer->cachedMax.y, data->data.Get(j).y)};
                    vertex.texcoord = {data->data.Get(j).x / renderer->cachedMax.x, Math::Min(renderer->cachedMax.y, data->data.Get(j).y) / renderer->cachedMax.y};
                    renderer->plots[i]->cachedVertices.Add(vertex);

                    // 3
                    vertex.position = {data->data.Get(j + 1).x, renderer->cachedMax.y - Math::Min(renderer->cachedMax.y, data->data.Get(j + 1).y)};
                    vertex.texcoord = {data->data.Get(j + 1).x / renderer->cachedMax.x, Math::Min(renderer->cachedMax.y, data->data.Get(j + 1).y) / renderer->cachedMax.y};
                    renderer->plots[i]->cachedVertices.Add(vertex);
                }
                break;
            }
            case UIPlotType::eLineFill: {
                Graphics::UIGeometryVertex vertex;
                for (USize j = 0; j < data->data.Size() - 1; j++) {
                    UInt32 baseVertex = static_cast<UInt32>(renderer->plots[i]->cachedVertices.Size());

                    // 0
                    vertex.position = {data->data.Get(j).x, renderer->cachedMax.y};
                    vertex.texcoord = {data->data.Get(j).x / renderer->cachedMax.x, 0};
                    renderer->plots[i]->cachedVertices.Add(vertex);

                    // 1
                    vertex.position = {data->data.Get(j).x, renderer->cachedMax.y - Math::Min(renderer->cachedMax.y, data->data.Get(j).y)};
                    vertex.texcoord = {data->data.Get(j).x / renderer->cachedMax.x, Math::Min(renderer->cachedMax.y, data->data.Get(j).y) / renderer->cachedMax.y};
                    renderer->plots[i]->cachedVertices.Add(vertex);

                    // 2
                    vertex.position = {data->data.Get(j + 1).x, renderer->cachedMax.y};
                    vertex.texcoord = {data->data.Get(j + 1).x / renderer->cachedMax.x, 0};
                    renderer->plots[i]->cachedVertices.Add(vertex);

                    // 3
                    vertex.position = {data->data.Get(j + 1).x, renderer->cachedMax.y - Math::Min(renderer->cachedMax.y, data->data.Get(j + 1).y)};
                    vertex.texcoord = {data->data.Get(j + 1).x / renderer->cachedMax.x, Math::Min(renderer->cachedMax.y, data->data.Get(j + 1).y) / renderer->cachedMax.y};
                    renderer->plots[i]->cachedVertices.Add(vertex);

                    // Indices
                    renderer->plots[i]->cachedIndices.Add(
                            baseVertex + 1, baseVertex + 2, baseVertex + 0, baseVertex + 1, baseVertex + 3, baseVertex + 2
                            //baseVertex + 0, baseVertex + 2, baseVertex + 1, baseVertex + 2, baseVertex + 3, baseVertex + 1
                    );
                }
                break;
            }
            case UIPlotType::ePoint: {
                break;
            }
        }
    }

    return true;
}

UIGraph::Renderer::Renderer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIRenderComponent(registry, outer, host) {

}

void UIGraph::Renderer::Build(Graphics::UIDrawPacketBuffer *buffer) {
    auto transform = GetElement()->GetTransform();

    // Get desired size
    Math::Vector2 desiredSize = transform->GetDesiredSize();

    // Background texture
    if (texture) {
        Graphics::UIDrawPacket background;
        background.brush = Graphics::UIBrushTexture(Math::Vector4(1, 1, 1, 1), texture, GHA::Samplers::noMip);
        background.transform = Math::Matrix4::Translate({-6, -6, 0}) * transform->GetPixelTransform();
        background.geometry = Graphics::UIGeometryMarginBox(desiredSize + Math::Vector2(12, 12), {4, 4});
        background.viewport = transform->GetViewport();
        buffer->Add(background);
    }

    // Grid lines
    if (cachedLines.Size() > 0) {
        Graphics::UIDrawPacket grid;
        grid.brush = Graphics::UIBrushSolid({0.5f, 0.5f, 0.5f, 1.0f});
        grid.transform = Math::Matrix4::Translate({0, 0, 1}) * transform->GetPixelTransform();
        Graphics::UIGeometryLine gridLines;
        gridLines.connected = false;
        gridLines.vertices = cachedLines;
        gridLines.width = 1.0f;
        grid.geometry = gridLines;
        grid.viewport = transform->GetViewport();
        buffer->Add(grid);
    }

    // Plots
    for (UInt32 i = 0; i < plots.Size(); i++) {
        Graphics::UIDrawPacket packet;
        packet.transform = Math::Matrix4::Translate({0, 0, static_cast<float>(2 + i)}) * transform->GetPixelTransform();
        packet.viewport = transform->GetViewport();

        auto properties = plots[i]->properties;

        // Must not be empty
        if (plots[i]->cachedVertices.Size() == 0) {
            continue;
        }

        // ...
        switch (properties.type) {
            case UIPlotType::eLine: {
                packet.transform = Math::Matrix4::Scale(Math::Vector3(desiredSize / cachedMax, 1)) * packet.transform;

                Graphics::UIGeometryLine line;
                line.vertices = plots[i]->cachedVertices;
                line.connected = false;
                line.width = 1.0f;
                packet.geometry = line;
                packet.brush = properties.brush;
                buffer->Add(packet);
                break;
            }
            case UIPlotType::eLineFill: {
                packet.transform = Math::Matrix4::Scale(Math::Vector3(desiredSize / cachedMax, 1)) * packet.transform;

                Graphics::UIGeometryCustom custom;
                custom.vertices = plots[i]->cachedVertices;
                custom.indices = plots[i]->cachedIndices;
                packet.geometry = custom;
                packet.brush = properties.brush;
                buffer->Add(packet);
                break;
            }
            case UIPlotType::ePoint: {
                break;
            }
        }
    }
}
