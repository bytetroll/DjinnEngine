//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <Core/BitSet.h>
#include <Core/TypeAssignable.h>

namespace Djinn::GHA {
    enum class DeviceFlag {
        eDebug = 1,
    }; DJINN_CORE_BITSET(DeviceFlag);

    struct DeviceDesc {
        DeviceDesc& Flags(DeviceFlagSet value) {
            flags = value;
            return *this;
        }

        // Flags
        DeviceFlagSet flags;
    };
}