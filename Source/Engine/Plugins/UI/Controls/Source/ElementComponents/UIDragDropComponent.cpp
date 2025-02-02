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
 on 8/24/2018.
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