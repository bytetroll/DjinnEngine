//
// Created by Dev on 8/24/2018.
//

#include <UI/ElementComponents/UIDragDropComponent.h>
#include <UI/ElementComponents/UIInputComponent.h>
#include <UI/ElementComponents/IUITransformComponent.h>
#include <UI/UIInputActionBuffer.h>
#include <UI/IUIElement.h>

DJINN_NS2(UI);

UIDragDropComponent::UIDragDropComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElementComponent(registry, outer, host) {

}

void UIDragDropComponent::SetMovementTransform(IUITransformComponent *transform) {
    movementTransform = transform->GetWeakPtr();
}

void UIDragDropComponent::StaticInitialize() {
    // Must have an input component installed
    auto input = GetElement()->GetOrCreateFirstComponent<UIInputComponent>();

    // Set target
    movementTransform = GetElement()->GetTransform()->GetWeakPtr();

    // Press drag event
    input->mousePressed.Add(this, [=](const Game::InputEvent::MousePressed &event, UIInputActionBuffer *buffer) {
        isDragging = true;
        baseDragOffset = event.position;

        // Bind mouse
        buffer->Add(UIInputAction::BindMouseFocus(input->GetWeakPtr()));
    });

    // Release drag event
    input->mouseReleased.Add(this, [=](const Game::InputEvent::MouseReleased &event, IUIElement* element, UIInputActionBuffer *buffer) {
        if (isDragging) {
            isDragging = false;

            // Bind mouse
            buffer->Add(UIInputAction::UnbindMouseFocus());
        }
    });

    // Mouse moved drag
    input->mouseMoved.Add(this, [=](const Game::InputEvent::MouseMoved &event, UIInputActionBuffer *buffer) {
        if (isDragging) {
            auto delta = (event.position - baseDragOffset);
            if (delta.LengthSqr() > 0) {
                auto margin = movementTransform->GetMargin() + Math::Vector2(delta.x * dragMask.x, delta.y * dragMask.y);

                auto parent = movementTransform->GetParent();
                if (restrictedBounds && parent) {
                    auto bounds = parent->GetDesiredSize() - movementTransform->GetDesiredSize();
                    if (dragMask.x != 0) {
                        margin.x = Math::Clamp(margin.x, 0, bounds.x);
                    }
                    if (dragMask.y != 0) {
                        margin.y = Math::Clamp(margin.y, 0, bounds.y);
                    }
                }

                movementTransform->SetMargin(margin);
                baseDragOffset = event.position;
                movedEvents.Signal(this);
            }
        }
    });
}