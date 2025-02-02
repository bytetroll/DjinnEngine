//
// Created by Dev on 6/28/2018.
//

#include <Editor/Elements/MainLayout.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Game/IWorld.h>
#include <Base/DefaultLogs.h>
#include <UI/ElementComponents/IUITransformComponent.h>
#include <Game/IActorSystem.h>
#include <UI/Elements/UIDocking.h>
#include <UI/IUICanvas.h>
#include <Editor/LevelLayoutSceneState.h>
#include <Editor/ILevel.h>
#include <Data/IAssetHost.h>
#include <Data/IUIMLAsset.h>
#include <UI/IUIMLHost.h>

DJINN_NS2(Editor);

MainLayout::MainLayout(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMainLayout(registry, outer, host) {

}

void MainLayout::StaticInitialize() {
    Construct("Runtime/Editor/UI/MainLayout.uiml"_path);

    GetTransform()->SetScaling(UI::UIScaling::eFill, UI::UIScaling::eFill);
}

COM::Result MainLayout::AddLevel(ILevel *level) {
    // Create layout state
    LevelLayoutSceneState* state;
    if (!registry->CreateClass(this, &state) || !state->Initialize(level)) {
        ErrorLog(registry).Write("Failed to create scene");
        return COM::kError;
    }

    // Create canvas
    UI::IUICanvas* canvas;
    if (!level->GetGameWorld()->CreateScene(state, &canvas)) {
        ErrorLog(registry).Write("Failed to create scene");
        return COM::kError;
    }

    // Configure canvas
    {
        canvas->SetProxy(COM::CastChecked<UI::IUICanvas>(GetScene()));
    }

    // Get asset worldHost
    Data::IAssetHost* host;
    CheckMsg(registry->GetPipeline()->GetInterface(&host), Error, "Expected asset worldHost to be installed");

    // Get uiml worldHost
    UI::IUIMLHost* uiml;
    CheckMsg(world->GetPipeline()->GetInterface(&uiml), Error, "Expected UIML worldHost to be installed");

    // Get docker
    auto docking = FindChild<UI::UIDocking>("level-docker");
    if (!docking) {
        return COM::kError;
    }

    // Build from path
    Check(uiml->Build(canvas, host->CompileAssetAsync<Data::IUIMLAsset>("Runtime/Editor/UI/LevelLayout.uiml"_path).Get(), this, [&](IUIElement* element) {
        // We're adding this element to another world, we know it's safe
        element->SetCrossWorldReferencing(true);

        docking->AddChild(element->GetTransform());
    }));

    // Track
    LevelInstance instance;
    instance.level = level;
    instance.canvas = canvas;
    instances.Add(instance);

    // OK
    return COM::kOK;
}
