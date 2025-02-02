//
// Created by Dev on 1/12/2018.
//

#include <Game/WindowInputDevice.h>
#include <Platform/Window.h>
#include <Game/InputEvent.h>

DJINN_NS2(Game);

WindowInputDevice::WindowInputDevice(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IInputDevice(registry, outer, host) {

}

COM::Result WindowInputDevice::PoolEvents(UInt32 &count, InputEvent *out) {
    if (out) {
        Check(window->PoolEvents(count, stagedEvents.Ptr()));

        for (USize i = 0; i < count; i++) {
            auto &event = stagedEvents[i];
            auto &inputEvent = out[i];

            // Translate
            switch (event.type) {
                default:
                    break;
                case Platform::WindowEventType::eCharacter:
                    inputEvent.character = {};
                    inputEvent.character.virtualKeyCode = event.character.virtualKeyCode;
                    inputEvent.character.key = event.character.key;
                    break;
                case Platform::WindowEventType::eKeyPressed:
                    inputEvent.keyPressed = {};
                    inputEvent.keyPressed.key = event.keyPressed.key;
                    break;
                case Platform::WindowEventType::eKeyReleased:
                    inputEvent.keyReleased = {};
                    inputEvent.keyReleased.key = event.keyReleased.key;
                    break;
                case Platform::WindowEventType::eKeyDown:
                    inputEvent.keyDown = {};
                    inputEvent.keyDown.key = event.keyDown.key;
                    break;
                case Platform::WindowEventType::eMousePressed:
                    inputEvent.mousePressed = {};
                    inputEvent.mousePressed.virtualKeyCode = event.mousePressed.virtualKeyCode;
                    inputEvent.mousePressed.position = Math::Vector2(event.mousePressed.position.x, event.mousePressed.position.y);
                    break;
                case Platform::WindowEventType::eMouseReleased:
                    inputEvent.mouseReleased = {};
                    inputEvent.mouseReleased.virtualKeyCode = event.mouseReleased.virtualKeyCode;
                    inputEvent.mouseReleased.position = Math::Vector2(event.mouseReleased.position.x, event.mouseReleased.position.y);
                    break;
                case Platform::WindowEventType::eMouseMoved:
                    inputEvent.mouseMoved = {};
                    inputEvent.mouseMoved.position = Math::Vector2(event.mouseMoved.position.x, event.mouseMoved.position.y);
                    inputEvent.mouseMoved.delta = Math::Vector2(event.mouseMoved.delta.x, event.mouseMoved.delta.y);
                    break;
                case Platform::WindowEventType::eMouseScroll:
                    inputEvent.mouseScroll = {};
                    inputEvent.mouseScroll.value = event.mouseScroll.value;
                    break;
                case Platform::WindowEventType::eClosed:
                    inputEvent.type = InputEventType::eExit;
                    break;
            }
        }
    } else {
        Check(window->PoolEvents(count, nullptr));
        stagedEvents.Resize(count);
    }
    return COM::kOK;
}


COM::Result WindowInputDevice::Initialize(Platform::Window *window) {
    this->window = window;

    // OK
    return COM::kOK;
}
