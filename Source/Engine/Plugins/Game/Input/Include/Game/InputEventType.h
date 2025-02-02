//
// Created by Dev on 1/12/2018.
//
#pragma once

namespace Djinn::Game {
    enum class InputEventType {
        eNone,

        /// Any event, used for event tracking
                eAny,

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

        /// A mouse button was held down
                eMouseDown,

        /// A mouse button was released
                eMouseReleased,

        /// The mouse was moved
                eMouseMoved,

        /// The mouse was scrolled
                eMouseScroll,

        /// Special event, signals the exit of the application
                eExit,

        /// A custom event
        /// Note this should be avoided
                eCustom,
    };
}