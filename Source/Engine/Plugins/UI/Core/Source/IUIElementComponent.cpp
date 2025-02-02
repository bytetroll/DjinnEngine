//
// Created by Dev on 8/22/2018.
//

#include <UI/IUIElementComponent.h>
#include <UI/IUIElement.h>

DJINN_NS2(UI);

IUIElement *IUIElementComponent::GetElement() const {
    return static_cast<IUIElement*>(GetActor());
}
