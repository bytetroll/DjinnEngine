//
// Created by Dev on 8/22/2018.
//

#include <UI/IUIElement.h>
#include <UI/Style.h>
#include <UI/IUICanvas.h>
#include <UI/ElementComponents/IUIRenderComponent.h>
#include <UI/ElementComponents/UILeafTransformComponent.h>

DJINN_NS2(UI);

IUIElement::IUIElement(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : IActor(registry, outer, host) {
}

void IUIElement::SetTransform(IUITransformComponent *transform) {
    DJINN_ASSERT(!transform->GetParent(), "Cannot set transform when element transform already has a parent");
    this->transform = transform->GetWeakPtr();
}

void IUIElement::StaticInitialize() {
    // Default transform
    SetTransform(CreateComponent<UILeafTransformComponent>());
}

IUIElement* IUIElement::FindChild(const Core::StringView& name) {
    if (auto transform = GetTransform()->FindChild(name)) {
        return transform->GetElement();
    }
    return nullptr;
}

void IUIElement::MarkAsUncached() {
    needsCaching = true;
    transform->MarkAsDirty();
}

void IUIElement::SetStyleState(const StyleStateID &id) {
    styleState = id;
    MarkAsUnstyled();
}

void IUIElement::RemoveStyleState() {
    styleState = {};
    MarkAsUnstyled();
}

void IUIElement::MarkAsUnstyled() {
    needsStyling = true;
    transform->MarkAsDirty();
}

void IUIElement::MarkAsStyled() {
    needsStyling = false;
}

void IUIElement::ApplyStyle() {
    // Get style
    style = COM::CastChecked<IUICanvas>(GetScene())->GetStyle();

    // Compile
    elementStyle = style->CompileStyle(this);

    // Apply styling
    StyleEvent(elementStyle);

    // Apply component styling
    for (auto &&com : components) {
        if (auto element = COM::Cast<IUIElementComponent>(com)) {
            element->StyleEvent(elementStyle);
        }
    }

    // OK
    MarkAsStyled();
}

void IUIElement::AddStyleClass(const StyleClassID &id) {
    styleClasses.Add(id);
}

void IUIElement::RemoveStyleClass(const StyleClassID &id) {
    styleClasses.RemoveValue(id);
}

Style *IUIElement::RequestStyle(Style *style) {
    return style;
}

