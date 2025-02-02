//
// Created by Dev on 7/12/2018.
//
#pragma once

#include <Core/Delegate.h>

namespace Djinn::Editor {
    struct Command {
        Command(const char* name = nullptr) : name(name) {

        }

        Command& Name(const char* value = nullptr) {
            name = value;
            return *this;
        }

        Command& Action(const Core::Delegate<void()>& value) {
            action = value;
            return *this;
        }

        Command& Undo(const Core::Delegate<void()>& value) {
            undo = value;
            return *this;
        }

        const char* name = nullptr;
        Core::Delegate<void()> action;
        Core::Delegate<void()> undo;
    };
}