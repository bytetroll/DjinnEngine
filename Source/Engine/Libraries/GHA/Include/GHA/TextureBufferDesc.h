//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <Core/TypeAssignable.h>
#include "IResource.h"
#include "Format.h"

namespace Djinn::GHA {
    struct TextureBufferDesc {
        TextureBufferDesc& Stride(USize value) {
            stride = value;
            return *this;
        }

        TextureBufferDesc& ElementCount(UInt32 value) {
            elementCount = value;
            return *this;
        }

        // Stride of each element
        USize stride;

        // Number of elements
        UInt32 elementCount;
    };
}
