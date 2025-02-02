//
// Created by Dev on 11/23/2017.
//
#pragma once

#include "AllocationSize.h"

namespace Djinn::GHA {
    struct StructuredBufferDesc {
        StructuredBufferDesc& Size(const AllocationSize& value) {
            size = value;
            return *this;
        }

        // Total size of this buffer
        AllocationSize size;
    };
}