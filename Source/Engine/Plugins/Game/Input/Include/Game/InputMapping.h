//
// Created by Dev on 1/15/2018.
//
#pragma once

#include <Core/FixedArray.h>
#include <Platform/WindowEvent.h>

namespace Djinn::Game {
    struct InputMapping {
        InputMapping &Key(Platform::Key value) {
            key = value;
            return *this;
        }

        InputMapping &Modifiers(const Core::FixedArray8<Platform::VirtualKeyCode> &value) {
            modifiers = value;
            return *this;
        }

        InputMapping &AddModifier(Platform::VirtualKeyCode value) {
            modifiers.Add(value);
            return *this;
        }

        /// Key
        Platform::Key key;

        /// All enabled modifiers
        Core::FixedArray8<Platform::VirtualKeyCode> modifiers;
    };
}