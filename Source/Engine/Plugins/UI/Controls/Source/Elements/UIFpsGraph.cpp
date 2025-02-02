//
// Created by Dev on 8/23/2018.
//

#include <UI/Elements/UIFpsGraph.h>
#include <UI/Elements/UIGraph.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <Game/IActorSystem.h>
#include <Game/IGameSystem.h>
#include <UI/Elements/UIImage.h>
#include <UI/Elements/UILabel.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>
#include <UI/ElementComponents/UIInputComponent.h>

DJINN_NS2(UI);

UIFpsGraph::UIFpsGraph(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UIFpsGraph::StaticInitialize() {
    callback.SetCallback(Bindff(Update));
    callback.Register(this);

    // Get game system
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&gameSystem), Error, "Expected game system to be installed");

    // Defaults
    count = 100;

    CheckVoid(GetSystem()->CreateActor(GetScene(), &graph));

    // Create graph component
    graph->SetGridUnitSnap(10.0f);

    // Rapid FPS
    UIPlotProperties properties;
    properties.type = UIPlotType::eLineFill;
    properties.brush = Graphics::UIBrushGradient({0, 0}, {1, 0, 0, 1}, {0, 1}, {0, 1, 0, 1});
    graph->AddPlot(properties);

    // Smooth FPS
    properties.type = UIPlotType::eLine;
    properties.brush = Graphics::UIBrushSolid({1.0f, 1.0f, 1.0f, 1.0f});
    graph->AddPlot(properties);

    // Variance
    properties.type = UIPlotType::eLine;
    properties.brush = Graphics::UIBrushSolid({0, 0, 0, 1});
    graph->AddPlot(properties);

    // Base data
    {
        auto data = graph->MapPlotData(0);
        for (USize i = 0; i < count; i++) {
            data->data.Add({static_cast<float>(i), 0});
        }
        graph->UnmapPlotData(data);

        auto dataSmooth = graph->MapPlotData(1);
        for (USize i = 0; i < count; i++) {
            dataSmooth->data.Add({static_cast<float>(i), 0});
        }
        graph->UnmapPlotData(dataSmooth);

        auto dataVariance = graph->MapPlotData(2);
        for (USize i = 0; i < count; i++) {
            dataVariance->data.Add({static_cast<float>(i), 0});
        }
        graph->UnmapPlotData(dataVariance);
    }

    SetTransform(graph->GetTransform());
}

void UIFpsGraph::Update(double delta) {
    auto data = graph->MapPlotData(0);
    for (USize i = 0; i < count - 1; i++) {
        data->data.Get(i) = data->data.Get(i + 1);
        data->data.Get(i).x = i;
    }
    data->data.Last() = {static_cast<float>(count - 1), static_cast<float>(1.0 / gameSystem->GetDelta())};
    graph->UnmapPlotData(data);

    double weight = 0.1;
    avgDelta = (1.0 - weight) * avgDelta + weight * gameSystem->GetDelta();

    auto dataSmooth = graph->MapPlotData(1);
    for (USize i = 0; i < count - 1; i++) {
        dataSmooth->data.Get(i) = dataSmooth->data.Get(i + 1);
        dataSmooth->data.Get(i).x = i;
    }
    dataSmooth->data.Last() = {static_cast<float>(count - 1), static_cast<float>(1.0 / avgDelta)};
    graph->UnmapPlotData(dataSmooth);

    auto dataVariance = graph->MapPlotData(2);
    for (USize i = 0; i < count - 1; i++) {
        dataVariance->data.Get(i) = dataVariance->data.Get(i + 1);
        dataVariance->data.Get(i).x = i;
    }
    dataVariance->data.Last() = {static_cast<float>(count - 1), static_cast<float>(Math::Abs(1.0 / gameSystem->GetDelta() - 1.0 / lastDelta))};
    graph->UnmapPlotData(dataVariance);

    lastDelta = gameSystem->GetDelta();
}

