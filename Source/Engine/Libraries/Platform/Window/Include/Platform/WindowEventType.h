//
// Created by Dev on 1/12/2018.
//
#pragma once

namespace Djinn::Platform {
    enum class WindowEventType {
        eNone,

        /// A character event
                eCharacter,

        /// A key was pressed
                eKeyPressed,

        /// A key was released
                eKeyReleased,

        /// A key was held down
                eKeyDown,

        /// A mouse button was pressed
                eMousePressed,

        /// A mouse button was released
                eMouseReleased,

        /// The mouse was moved
                eMouseMoved,

        /// The mouse was scrolled
                eMouseScroll,

        /// Closed event
                eClosed,
    };
}