//
// Created by Dev on 8/24/2018.
//

#include <UI/ElementComponents/UIResizeComponent.h>
#include <UI/ElementComponents/UIInputComponent.h>
#include <UI/ElementComponents/IUITransformComponent.h>
#include <UI/UIInputActionBuffer.h>
#include <UI/IUIElement.h>

DJINN_NS2(UI);

UIResizeComponent::UIResizeComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElementComponent(registry, outer, host) {

}

void UIResizeComponent::StaticInitialize() {
    // Must have an input component installed
    auto input = GetElement()->GetOrCreateFirstComponent<UIInputComponent>();

    // Set target
    resizeTransform = GetElement()->GetTransform()->GetWeakPtr();

    // Press drag event
    input->mousePressed.Add(this, [=](const Game::InputEvent::MousePressed &event, UIInputActionBuffer *buffer) {
        Math::Vector2 cursor = {static_cast<float>(event.position.x), static_cast<float>(event.position.y)};


        // Bottom Right Cornor
        if (faces & UIResizeFace::eBottomRight && (cursor - (resizeTransform->GetAbsolutePosition() + resizeTransform->GetDesiredSize())).Length() <= treshold) {
            marginMask = {0, 0};
            desiredSizeMask = {1, 1};
        }

            // Bottom Left Cornor
        else if (faces & UIResizeFace::eBottomLeft && (cursor - (resizeTransform->GetAbsolutePosition() + Math::Vector2(0, resizeTransform->GetDesiredSize().y))).Length() <= treshold) {
            marginMask = {1, 0};
            desiredSizeMask = {0, 1};
        }

            // Top Right Cornor
        else if (faces & UIResizeFace::eTopRight && (cursor - (resizeTransform->GetAbsolutePosition() + Math::Vector2(resizeTransform->GetDesiredSize().x, 0))).Length() <= treshold) {
            marginMask = {0, 1};
            desiredSizeMask = {1, 0};
        }

            // Top Left Cornor
        else if (faces & UIResizeFace::eTopLeft && (cursor - resizeTransform->GetAbsolutePosition()).Length() <= treshold) {
            marginMask = {1, 1};
            desiredSizeMask = {0, 0};
        }

            // Top edge
        else if (faces & UIResizeFace::eTop && (cursor.y - resizeTransform->GetAbsolutePosition().y) <= treshold) {
            marginMask = {0, 1};
            desiredSizeMask = {0, 0};
        }

            // Left edge
        else if (faces & UIResizeFace::eLeft && (cursor.x - resizeTransform->GetAbsolutePosition().x) <= treshold) {
            marginMask = {1, 0};
            desiredSizeMask = {0, 0};
        }

            // Bottom edge
        else if (faces & UIResizeFace::eBottom && ((resizeTransform->GetAbsolutePosition().y + resizeTransform->GetDesiredSize().y) - cursor.y) <= treshold) {
            marginMask = {0, 0};
            desiredSizeMask = {0, 1};
        }

            // Right edge
        else if (faces & UIResizeFace::eRight && ((resizeTransform->GetAbsolutePosition().x + resizeTransform->GetDesiredSize().x) - cursor.x) <= treshold) {
            marginMask = {0, 0};
            desiredSizeMask = {1, 0};
        }

            // No match
        else {
            return;
        }

        // ...
        isResizing = true;
        baseDragOffset = event.position;

        // Bind mouse
        buffer->Add(UIInputAction::BindMouseFocus(input->GetWeakPtr()));
    });

    // Release drag event
    input->mouseReleased.Add(this, [=](const Game::InputEvent::MouseReleased &event, IUIElement* element, UIInputActionBuffer *buffer) {
        if (isResizing) {
            isResizing = false;

            // Bind mouse
            buffer->Add(UIInputAction::UnbindMouseFocus());
        }
    });

    // Mouse moved drag
    input->mouseMoved.Add(this, [=](const Game::InputEvent::MouseMoved &event, UIInputActionBuffer *buffer) {
        if (isResizing) {
            auto delta = event.position - baseDragOffset;
            if (delta.LengthSqr() > 0) {
                auto offset = Math::Vector2(static_cast<float>(delta.x), static_cast<float>(delta.y));
                resizeTransform->SetMargin(resizeTransform->GetMargin() + offset * marginMask);
                resizeTransform->SetDesiredSize(resizeTransform->GetDesiredSize() + offset * desiredSizeMask);
                resizeTransform->SetDesiredSize(resizeTransform->GetDesiredSize() - offset * marginMask);
                baseDragOffset = event.position;
            }
        }
    });
}

void UIResizeComponent::SetResizeTransform(IUITransformComponent *transform) {
    resizeTransform = transform->GetWeakPtr();
}
