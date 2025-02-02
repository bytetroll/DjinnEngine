//
// Created by Dev on 1/10/2018.
//
#pragma once

#include <Core/String.h>

namespace Djinn::Base {
    struct ApplicationInfo {
        ApplicationInfo& Name(const Core::String& value) {
            name = value;
            return *this;
        }

        /// Name of this application
        Core::String name;
    };
}