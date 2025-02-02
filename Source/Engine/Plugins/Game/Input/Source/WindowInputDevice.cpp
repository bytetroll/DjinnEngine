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
 on 1/12/2018.
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
