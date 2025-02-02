//
// Created by Dev on 1/12/2018.
//
#pragma once

#include <Core/Types.h>
#include <Platform/VirtualKeyCode.h>
#include <Platform/WindowEvent.h>
#include <Core/Memory.h>
#include <Core/WeakPtr.h>
#include <Math/Vector2.h>
#include "UIInputActionType.h"

namespace Djinn::UI {
    class UIInputComponent;

    struct UIInputAction {
        UIInputAction() : type(UIInputActionType::eNone) {
            // ...
        }

        UIInputAction(const UIInputAction &other) {
            Core::Memory::Copy(this, &other, sizeof(UIInputAction));
        }

        /// Bind the mouse focus
        /// \param com
        /// \return
        static UIInputAction BindMouseFocus(const Core::WeakPtr<UIInputComponent> &com) {
            UIInputAction action;
            action.com = com;
            action.type = UIInputActionType::eBindMouseFocus;
            return action;
        }

        /// Bind the keyboard focus
        /// \param com
        /// \return
        static UIInputAction BindKeyboardFocus(const Core::WeakPtr<UIInputComponent> &com) {
            UIInputAction action;
            action.com = com;
            action.type = UIInputActionType::eBindKeyboardFocus;
            return action;
        }

        /// Unbind bound mouse focus
        /// \return
        static UIInputAction UnbindMouseFocus() {
            UIInputAction action;
            action.type = UIInputActionType::eUnbindMouseFocus;
            return action;
        }

        /// Unbind bound keyboard focus
        /// \return
        static UIInputAction UnbindKeyboardFocus() {
            UIInputAction action;
            action.type = UIInputActionType::eUnbindKeyboardFocus;
            return action;
        }

        /// Set the mouse position
        /// \param pos
        /// \return
        static UIInputAction SetMousePosition(const Math::Vector2& pos) {
            UIInputAction action;
            action.setMousePosition.position = pos;
            action.type = UIInputActionType::eSetMousePosition;
            return action;
        }

        /// Action data
        union {
            struct {
                Math::Vector2 position;
            } setMousePosition;
        };

        /// Type of action
        UIInputActionType type;

        /// Component this action targets
        Core::WeakPtr<UIInputComponent> com;
    };
}