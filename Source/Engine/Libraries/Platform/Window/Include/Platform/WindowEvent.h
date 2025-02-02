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
#pragma once

#include <Core/Types.h>
#include <Math/Vector2I.h>
#include "WindowEventType.h"
#include "VirtualKeyCode.h"
#include "KeyCode.h"

namespace Djinn::Platform {
    union WindowEvent {
        WindowEvent() : type(WindowEventType::eNone) {
            // ...
        }

        /// Type wrapper
        WindowEventType type;

        /// Key pressed event
        struct Character {
            WindowEventType type = WindowEventType::eCharacter;
            VirtualKeyCode virtualKeyCode = Platform::VirtualKeyCode::eCharacter;
            char key;
        } character;

        /// Key pressed event
        struct KeyPressed {
            WindowEventType type = WindowEventType::eKeyPressed;
            Key key;
        } keyPressed;

        /// Key released event
        struct KeyReleased {
            WindowEventType type = WindowEventType::eKeyReleased;
            Key key;
        } keyReleased;

        /// Key released event
        struct KeyDown {
            WindowEventType type = WindowEventType::eKeyDown;
            Key key;
        } keyDown;

        /// Mouse pressed event
        struct MousePressed {
            WindowEventType type = WindowEventType::eMousePressed;
            VirtualKeyCode virtualKeyCode = VirtualKeyCode::eMouseLeft;
            Math::Vector2I position;
        } mousePressed;

        /// Mouse pressed event
        struct MouseReleased {
            WindowEventType type = WindowEventType::eMouseReleased;
            VirtualKeyCode virtualKeyCode = VirtualKeyCode::eMouseLeft;
            Math::Vector2I position;
        } mouseReleased;

        /// Key press event
        struct MouseMoved {
            WindowEventType type = WindowEventType::eMouseMoved;
            Math::Vector2I position;
            Math::Vector2I delta;
        } mouseMoved;

        /// Key press event
        struct MouseScroll {
            WindowEventType type = WindowEventType::eMouseScroll;
            float value = 0.0f;
        } mouseScroll;
    };
}