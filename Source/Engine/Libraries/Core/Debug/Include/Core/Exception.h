//
// Created by Dev on 5/27/2018.
//
#pragma once

#include <Core/String.h>

namespace Djinn::Debug {
    struct Exception {
        Exception(const Core::String& what) : what(what) {

        }

        /// What went wrong
        Core::String what;
    };

    /// Declare a shorthand exception
#define DJINN_EXCEPTION(NAME, MESSAGE) struct E##NAME : public Djinn::Debug::Exception { E##NAME() : Djinn::Debug::Exception(MESSAGE) { } };
}