//
// Created by Dev on 8/23/2018.
//

#include <UI/Elements/CanvasLayout.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>

DJINN_NS2(UI);

CanvasLayout::CanvasLayout(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void CanvasLayout::StaticInitialize() {
    auto layout = CreateComponent<UILayoutTransformComponent>();
    SetTransform(layout);
}

void CanvasLayout::AddChild(IUITransformComponent *child) {
    auto layout = static_cast<UILayoutTransformComponent *>(GetTransform());

    layout->AddChild(child);
}

void CanvasLayout::RemoveChild(IUITransformComponent *child) {
    auto layout = static_cast<UILayoutTransformComponent *>(GetTransform());

    layout->AddChild(child);
}
