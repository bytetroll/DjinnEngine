//
// Created by Dev on 7/11/2018.
//

#include <Editor/Elements/Properties.h>
#include <Base/DefaultLogs.h>
#include <UI/ElementComponents/IUITransformComponent.h>
#include <UI/Elements/UIOutputDisplay.h>
#include <Game/ActorSystem.h>
#include <Game/ICamera.h>
#include <Game/IWorld.h>
#include <Game/IScene.h>
#include <Editor/ILevel.h>
#include <Editor/IPropertiesHost.h>
#include <Editor/ILevelLayoutSceneState.h>

DJINN_NS2(Editor);

Properties::Properties(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IProperties(registry, outer, host) {

}

void Properties::StaticInitialize() {
    Construct("Runtime/Editor/UI/Properties.uiml"_path);

    auto layout = GetLayout();

    layout->SetScaling(UI::UIScaling::eFill, UI::UIScaling::eFill);
}

void Properties::Created() {
    // Get state
    auto state = COM::Cast<ILevelLayoutSceneState>(GetScene()->GetState());
    if (!state) {
        ErrorLog(this).Write("Properties created within a non level layout scene");
        return;
    }

    // Get the worldHost
    CheckMsgVoid(world->GetPipeline()->GetInterface(&host), Error, "Expected properties worldHost to be installed");

    // Register this
    CheckVoid(host->Register(this));
}
