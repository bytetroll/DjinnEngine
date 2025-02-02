//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <Core/BitSet.h>
#include <Core/TypeAssignable.h>

namespace Djinn::GHA {
    enum class InstanceFlag {
        eDebug = 1,
    }; DJINN_CORE_BITSET(InstanceFlag);

    struct InstanceDesc {
        InstanceDesc& Flags(InstanceFlagSet value) {
            flags = value;
            return *this;
        }

        // Flags
        InstanceFlagSet flags;
    };
}