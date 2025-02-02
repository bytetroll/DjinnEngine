//
// Created by Dev on 12/11/2017.
//
#pragma once

#include <Core/String.h>

namespace Djinn::Platform {
    struct WindowDesc {
        WindowDesc& Title(const Core::String& value) {
            title = value;
            return *this;
        }

        WindowDesc& Width(UInt32 value) {
            width = value;
            return *this;
        }

        WindowDesc& Height(UInt32 value) {
            height = value;
            return *this;
        }

        // Width of this window
        UInt32 width = 640;

        // Height of this window
        UInt32 height = 360;

        // Title of this window
        Core::String title = "DjinnEngine";
    };
}