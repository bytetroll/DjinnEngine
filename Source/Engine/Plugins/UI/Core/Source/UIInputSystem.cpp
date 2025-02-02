//
// Created by Dev on 8/23/2018.
//

#include <UI/UIInputSystem.h>
#include <UI/IUICanvas.h>
#include <UI/ElementComponents/IUITransformComponent.h>
#include <Game/IInputSystem.h>
#include <UI/ElementComponents/UIInputComponent.h>
#include <Base/DefaultLogs.h>
#include <Base/PipelineBuilder.h>
#include <Game/IActorSystem.h>

DJINN_NS2(UI);

UIInputSystem::UIInputSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : IUIInputSystem(registry, outer, host) {

}

COM::Result UIInputSystem::Initialize() {
    // Diagnostic
    InfoLog(this).Write("Initialized");

    // Get input system
    if (!registry->GetPipeline()->GetInterface(&inputSystem)) {
        ErrorLog(this).Write("UI Input component system expects an installed input system");
        return COM::kError;
    }

    // Bind generic events
    {
        Check(inputSystem->BindEvent(Game::InputEventType::eMouseMoved, [&](const Game::InputEvent &event) {
            Platform::LockGuard guard(mutex);
            screenMousePosition = event.mouseMoved.position;
            mouseMovedBuffer.Add(event.mouseMoved);
        }, nullptr));

        Check(inputSystem->BindEvent(Game::InputEventType::eMousePressed, [&](const Game::InputEvent &event) {
            Platform::LockGuard guard(mutex);
            mousePressedBuffer.Add(event.mousePressed);
        }, nullptr));

        Check(inputSystem->BindEvent(Game::InputEventType::eMouseReleased, [&](const Game::InputEvent &event) {
            Platform::LockGuard guard(mutex);
            mouseReleasedBuffer.Add(event.mouseReleased);
        }, nullptr));

        Check(inputSystem->BindCharacter([&](const Game::InputEvent::Character &event) {
            Platform::LockGuard guard(mutex);
            characterBuffer.Add(event);
        }, nullptr));
    }

    // OK
    return COM::kOK;
}

void UIInputSystem::Register(IUICanvas *canvas) {
    canvases.Add(canvas->GetWeakPtr());
}

void UIInputSystem::Deregister(IUICanvas *canvas) {
    canvases.RemoveValue(canvas->GetWeakPtr());
}

COM::Result UIInputSystem::Configure(Base::PipelineBuilder &builder) {
    builder.Before<Game::IActorSystem>();
    return COM::kOK;
}

COM::Result UIInputSystem::Run(Game::StepMode mode, double delta) {
    Platform::LockGuard guard(mutex);

    // Handle actions in buffer
    {
        for (const auto &action : actionBuffer.GetActions()) {
            switch (action.type) {
                case UIInputActionType::eNone:
                    break;
                case UIInputActionType::eBindMouseFocus: {
                    focusedMouseInput = action.com;
                    break;
                }
                case UIInputActionType::eBindKeyboardFocus: {
                    focusedKeyboardInput = action.com;
                    break;
                }
                case UIInputActionType::eUnbindMouseFocus: {
                    focusedMouseInput = nullptr;
                    break;
                }
                case UIInputActionType::eUnbindKeyboardFocus: {
                    focusedKeyboardInput = nullptr;
                    break;
                }
                case UIInputActionType::eSetMousePosition: {
                    Sink(inputSystem->SetMousePosition(action.setMousePosition.position));
                    lastMousePosition = action.setMousePosition.position;
                    break;
                }
            }
        }
        actionBuffer.Flush();
    }

    // Find current focused canvas
    Core::WeakPtr<IUICanvas> focusedCanvas;
    {
        for (auto &&canvas : canvases) {
            if (canvas->Trace(screenMousePosition, &mousePosition)) {
                focusedCanvas = canvas;
                break;
            }
        }
    }

    // Needs flush?
    if (lastCanvas != focusedCanvas) {
        lastCanvas = focusedCanvas;
        lastMousePosition = mousePosition;
    }

    // Any canvas?
    if (!focusedCanvas) {
        return COM::kOK;
    }

    // Trace cursor element
    IUIElement *cursorElement = focusedCanvas->TraceChild(mousePosition, TransformTraceMode::eInput);
    /*if (cursorElement) {
        InfoLog(this).Write(cursorElement->GetClassName(), ", ", cursorElement->GetName());
    }*/

    // Attempt to get input component
    auto cursorInput = cursorElement ? cursorElement->GetFirstComponent<UIInputComponent>() : nullptr;

    // Hover events
    {
        // Leave hover
        if (lastHovered && (!cursorInput || (lastHovered != cursorInput))) {
            Game::InputEvent::MouseMoved mouseMoved;
            mouseMoved.position = mousePosition;

            lastHovered->endHover.Signal(lastHovered, mouseMoved, &actionBuffer);
            SignalPropagations(lastHovered->GetElement(), mouseMoved);
        }

        // Enter hover
        if (cursorInput && (!lastHovered || (lastHovered != cursorInput))) {
            Game::InputEvent::MouseMoved mouseMoved;
            mouseMoved.position = mousePosition;

            cursorInput->beginHover.Signal(cursorInput, mouseMoved, &actionBuffer);
            SignalPropagations(cursorInput->GetElement(), mouseMoved);
        }
    }

    // Mouse events
    {
        // Determine focused mouse component
        auto mouseInput = focusedMouseInput.Get();
        if (!mouseInput) {
            mouseInput = cursorInput;
        }

        // Must be valid
        if (mouseInput) {
            // Process all moved events
            for (auto &event : mouseMovedBuffer) {
                mouseInput->mouseMoved.Signal(mouseInput, event, &actionBuffer);
            }

            // Process all pressed events
            for (auto &event : mousePressedBuffer) {
                mouseInput->mousePressed.Signal(mouseInput, event, &actionBuffer);
            }

            // Process all released events
            for (auto &event : mouseReleasedBuffer) {
                mouseInput->mouseReleased.Signal(mouseInput, event, cursorElement, &actionBuffer);
            }
        }

        // Get propagation element
        auto propagationElement = mouseInput ? mouseInput->GetElement() : cursorElement;

        // Process all moved events
        for (auto &event : mouseMovedBuffer) {
            SignalPropagations(propagationElement, event);
        }

        // Process all pressed events
        for (auto &event : mousePressedBuffer) {
            SignalPropagations(propagationElement, event);
        }

        // Process all released events
        for (auto &event : mouseReleasedBuffer) {
            SignalPropagations(propagationElement, event);
        }


        // Flush events
        mouseMovedBuffer.Clear(false);
        mousePressedBuffer.Clear(false);
        mouseReleasedBuffer.Clear(false);
    }

    // Keyboard events
    {
        // Determine focused keyboard component
        auto keyboardInput = focusedKeyboardInput;
        if (!keyboardInput) {
            //mouseInput = cursorEntityInput;
        }

        // Must be valid
        if (keyboardInput) {
            // Process character events
            {
                for (auto &event : characterBuffer) {
                    keyboardInput->characterTyped.Signal(keyboardInput, event, &actionBuffer);
                    SignalPropagations(keyboardInput->GetElement(), event);
                }
                characterBuffer.Clear(false);
            }
        }
    }

    // ...
    lastMousePosition = mousePosition;
    lastHovered = cursorInput ? cursorInput->GetWeakPtr() : nullptr;
    return COM::kOK;
}

Game::StepModeSet UIInputSystem::GetStepMode() {
    return Game::StepMode::eOnce;
}

void UIInputSystem::SignalPropagations(IUIElement *element, const Game::InputEvent &event) {
    IUIElement *current = element;
    while (current) {
        if (auto input = current->GetFirstComponent<UIInputComponent>()) {
            input->propagated.Signal(element, event, element, &actionBuffer);
        }

        auto parent = current->GetTransform()->GetParent();
        current = parent ? parent->GetElement() : nullptr;
    }
}
