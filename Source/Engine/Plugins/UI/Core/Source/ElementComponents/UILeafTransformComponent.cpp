//
// Created by Dev on 8/20/2018.
//

#include <UI/ElementComponents/UILeafTransformComponent.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(UI);

UILeafTransformComponent::UILeafTransformComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUITransformComponent(registry, outer, host) {

}

Math::Vector2 UILeafTransformComponent::Measure() {
    return desiredSize;
}

void UILeafTransformComponent::Arrange() {
    if (!parent) {
        ErrorLog(this).Write("Leaf without parent");
    }
}

IUITransformComponent *UILeafTransformComponent::TraceChild(const Math::Vector2 &point, TransformTraceMode mode) {
    if (isTraceFallthrough && mode == TransformTraceMode::eInput) {
        return nullptr;
    }

    return IsPointInside(point) ? this : nullptr;
}

IUITransformComponent *UILeafTransformComponent::FindChild(const Core::StringView &name) {
    return nullptr;
}

bool UILeafTransformComponent::OnChildDetached(IUITransformComponent *child) {
    return false;
}


