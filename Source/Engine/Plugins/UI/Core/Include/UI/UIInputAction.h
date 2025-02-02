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