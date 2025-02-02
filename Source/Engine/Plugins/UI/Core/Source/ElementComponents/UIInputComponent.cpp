//
// Created by Dev on 8/23/2018.
//

#include <UI/ElementComponents/UIInputComponent.h>
#include <UI/UIInputActionBuffer.h>

DJINN_NS2(UI);

UIInputComponent::UIInputComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElementComponent(registry, outer, host) {

}

void UIInputComponent::EnsureDragHandles() {
    if (!dragHandles) {
        dragHandles = true;

        mousePressed.Add(this, [=](const Game::InputEvent::MousePressed &event, UIInputActionBuffer *actions) {
            isDragging = true;
            actions->Add(UIInputAction::BindMouseFocus(GetWeakPtr()));
        });

        mouseReleased.Add(this, [=](const Game::InputEvent::MouseReleased &event, IUIElement* element, UIInputActionBuffer *actions) {
            isDragging = false;
            actions->Add(UIInputAction::UnbindMouseFocus());
        });
    }
}

void UIInputComponent::BindDragged(Game::IActorSystemInstance *instance, const UIDragged &draggedEvent) {
    EnsureDragHandles();

    mouseMoved.Add(instance, [=](const Game::InputEvent::MouseMoved &event, UIInputActionBuffer *actions) {
        if (isDragging) {
            draggedEvent.Invoke(event, actions);
        }
    });
}
