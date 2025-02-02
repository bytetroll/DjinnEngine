//
// Created by Dev on 7/11/2018.
//

#include <Editor/Controls/Bootstrap.h>
#include <Base/DefaultLogs.h>
#include <UI/ElementComponents/IUITransformComponent.h>

DJINN_NS2(Editor);

Bootstrap::Bootstrap(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IControl(registry, outer, host) {

}

void Bootstrap::StaticInitialize() {
    Construct("Runtime/Editor/UI/Bootstrap.uiml"_path);

    GetTransform()->SetScaling(UI::UIScaling::eFill, UI::UIScaling::eFill);
}
