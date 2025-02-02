//
// Created by Dev on 7/11/2018.
//

#include <Editor/Elements/Hierarchy.h>
#include <Base/DefaultLogs.h>
#include <UI/ElementComponents/IUITransformComponent.h>
#include <UI/Elements/UIOutputDisplay.h>
#include <Game/ActorSystem.h>
#include <Game/ICamera.h>
#include <Game/IWorld.h>
#include <Game/IScene.h>
#include <Editor/ILevel.h>
#include <Editor/IHierarchyHost.h>
#include <Editor/ILevelLayoutSceneState.h>

DJINN_NS2(Editor);

Hierarchy::Hierarchy(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IHierarchy(registry, outer, host) {

}

void Hierarchy::StaticInitialize() {
    Construct("Runtime/Editor/UI/Hierarchy.uiml"_path);

    auto layout = GetLayout();

    layout->SetScaling(UI::UIScaling::eFill, UI::UIScaling::eFill);
}

void Hierarchy::Created() {
    // Get state
    auto state = COM::Cast<ILevelLayoutSceneState>(GetScene()->GetState());
    if (!state) {
        ErrorLog(this).Write("Hierarchy created within a non level layout scene");
        return;
    }

    // Get the worldHost
    CheckMsgVoid(world->GetPipeline()->GetInterface(&host), Error, "Expected hierarchy worldHost to be installed");

    // Register this
    CheckVoid(host->Register(this));
}
