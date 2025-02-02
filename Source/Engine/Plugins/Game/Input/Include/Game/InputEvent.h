//
// Created by Dev on 1/12/2018.
//
#pragma once

#include <Core/Types.h>
#include <Platform/VirtualKeyCode.h>
#include <Platform/WindowEvent.h>
#include <Math/Vector2I.h>
#include <Math/Vector2.h>
#include "InputEventType.h"

namespace Djinn::Game {
    union InputEvent {
        InputEvent() : type(InputEventType::eNone) {
            // ...
        }

        /// Type wrapper
        InputEventType type;

        /// Character event
        struct Character {
            InputEventType type = InputEventType::eCharacter;
            Platform::VirtualKeyCode virtualKeyCode = Platform::VirtualKeyCode::eCharacter;
            char key = 0;
        } character;

        /// Key pressed event
        struct KeyPressed {
            InputEventType type = InputEventType::eKeyPressed;
            Platform::Key key;
        } keyPressed;

        /// Key released event
        struct KeyReleased {
            InputEventType type = InputEventType::eKeyReleased;
            Platform::Key key;
        } keyReleased;

        /// Key released event
        struct KeyDown {
            InputEventType type = InputEventType::eKeyDown;
            Platform::Key key;
        } keyDown;

        /// Mouse pressed event
        struct MousePressed {
            InputEventType type = InputEventType::eMousePressed;
            Platform::VirtualKeyCode virtualKeyCode = Platform::VirtualKeyCode::eMouseLeft;
            Math::Vector2 position;
        } mousePressed;

        /// Mouse down event
        struct MouseDown {
            InputEventType type = InputEventType::eMouseDown;
            Platform::VirtualKeyCode virtualKeyCode = Platform::VirtualKeyCode::eMouseLeft;
            Math::Vector2 position;
        } mouseDown;

        /// Mouse pressed event
        struct MouseReleased {
            InputEventType type = InputEventType::eMouseReleased;
            Platform::VirtualKeyCode virtualKeyCode = Platform::VirtualKeyCode::eMouseLeft;
            Math::Vector2 position;
        } mouseReleased;

        /// Key press event
        struct MouseMoved {
            InputEventType type = InputEventType::eMouseMoved;
            Math::Vector2 position;
            Math::Vector2 delta;
        } mouseMoved;

        /// Key press event
        struct MouseScroll {
            InputEventType type = InputEventType::eMouseScroll;
            float value = 0.0f;
        } mouseScroll;

        /// Custom event
        struct Custom {
            InputEventType type = InputEventType::eCustom;
            char buffer[16];
        } custom;

        InputEvent(const Character &character) : character(character) {

        }

        InputEvent(const KeyPressed &keyPressed) : keyPressed(keyPressed) {

        }

        InputEvent(const KeyReleased &keyReleased) : keyReleased(keyReleased) {

        }

        InputEvent(const KeyDown &keyDown) : keyDown(keyDown) {

        }

        InputEvent(const MousePressed &mousePressed) : mousePressed(mousePressed) {

        }

        InputEvent(const MouseReleased &mouseReleased) : mouseReleased(mouseReleased) {

        }

        InputEvent(const MouseDown &mouseDown) : mouseDown(mouseDown) {

        }

        InputEvent(const MouseMoved &mouseMoved) : mouseMoved(mouseMoved) {

        }

        InputEvent(const MouseScroll &mouseScroll) : mouseScroll(mouseScroll) {

        }

        InputEvent(const Custom &custom) : custom(custom) {

        }

        /// Check if this event is a key event
        /// \return
        bool IsKeyEvent() const {
            return type == InputEventType::eCharacter ||
                   type == InputEventType::eKeyPressed ||
                   type == InputEventType::eKeyDown ||
                   type == InputEventType::eKeyReleased;
        }

        /// Check if this event is a mouse event
        /// \return
        bool IsMouseEvent() const {
            return type == InputEventType::eMouseDown ||
                   type == InputEventType::eMouseMoved ||
                   type == InputEventType::eMousePressed ||
                   type == InputEventType::eMouseReleased ||
                   type == InputEventType::eMouseScroll;
        }

        /// Check if this event is a focus event
        /// \return
        bool IsFocusEvent() const {
            return type == InputEventType::eMouseDown ||
                   type == InputEventType::eMousePressed;
        }
    };
}