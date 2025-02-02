//
// Created by Dev on 1/12/2018.
//
#pragma once

namespace Djinn::UI {
    enum class UIInputActionType {
        eNone,

        /// Bind mouse input focus
                eBindMouseFocus,

        /// Bind keyboard input focus
                eBindKeyboardFocus,

        /// Unbind mouse input focus
                eUnbindMouseFocus,

        /// Unbind keyboard input focus
                eUnbindKeyboardFocus,

        /// Set the mouse position
                eSetMousePosition,
    };
}