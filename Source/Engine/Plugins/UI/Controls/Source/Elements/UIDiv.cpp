//
// Created by Dev on 8/23/2018.
//

#include <UI/Elements/UIDiv.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>

DJINN_NS2(UI);

UIDiv::UIDiv(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UIDiv::StaticInitialize() {
    auto layout = CreateComponent<UILayoutTransformComponent>();
    SetTransform(layout);
}

void UIDiv::AddChild(IUITransformComponent *child) {
    auto layout = static_cast<UILayoutTransformComponent *>(GetTransform());

    layout->AddChild(child);
}

void UIDiv::RemoveChild(IUITransformComponent *child) {
    auto layout = static_cast<UILayoutTransformComponent *>(GetTransform());

    layout->AddChild(child);
}
