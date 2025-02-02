//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 8/22/2018.
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

