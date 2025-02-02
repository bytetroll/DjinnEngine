//
// Created by Dev on 1/12/2018.
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