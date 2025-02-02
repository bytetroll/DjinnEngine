//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <Core/TypeAssignable.h>
#include "AllocationSize.h"
#include "IResource.h"

namespace Djinn::GHA {
    struct ConstantBufferDesc {
        ConstantBufferDesc& AllocationSize(const AllocationSize& value) {
            size = value;
            return *this;
        }

        GHA::AllocationSize size;
    };
}
