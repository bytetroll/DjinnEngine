//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <Core/TypeAssignable.h>
#include "IResource.h"
#include "Format.h"

namespace Djinn::GHA {
    struct TextureBufferViewDesc {
        TextureBufferViewDesc& Format(GHA::Format value) {
            format = value;
            return *this;
        }

        // Format of this texture buffer
        GHA::Format format;
    };
}
