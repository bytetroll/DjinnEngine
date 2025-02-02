//
// Created by Dev on 7/11/2018.
//

#include <Editor/Elements/Viewport.h>
#include <Base/DefaultLogs.h>
#include <UI/ElementComponents/IUITransformComponent.h>
#include <UI/Elements/UIOutputDisplay.h>
#include <Game/ActorSystem.h>
#include <Game/ICamera.h>
#include <Game/IWorld.h>
#include <Game/IScene.h>
#include <Editor/ILevel.h>
#include <Editor/Actors/IViewportCamera.h>
#include <Editor/IViewportHost.h>
#include <Editor/ILevelLayoutSceneState.h>

DJINN_NS2(Editor);

Viewport::Viewport(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IViewport(registry, outer, host) {

}

void Viewport::StaticInitialize() {
    Construct("Runtime/Editor/UI/Viewport.uiml"_path);

    auto layout = GetLayout();

    layout->SetScaling(UI::UIScaling::eFill, UI::UIScaling::eFill);

    {
        CheckVoid(GetSystem()->CreateActor(GetScene(), &display));

        auto transform = display->GetTransform();
        transform->SetScaling(UI::UIScaling::eFill, UI::UIScaling::eFill);
        layout->AddChild(transform);
    }
}

void Viewport::Created() {
    // Get state
    auto state = COM::Cast<ILevelLayoutSceneState>(GetScene()->GetState());
    if (!state) {
        ErrorLog(this).Write("Viewport created within a non level layout scene");
        return;
    }

    // Get the worldHost
    CheckMsgVoid(world->GetPipeline()->GetInterface(&host), Error, "Expected viewport worldHost to be installed");

    // Register this
    CheckVoid(host->Register(this));
}
