//
// Created by Dev on 4/19/2018.
//
#pragma once

#include <Core/Unique.h>

namespace Djinn {
    namespace Async {
        DJINN_UNIQUEID(LaneID, UInt16);
    }

    /// Literal
    constexpr Async::LaneID operator "" _async(const char *str, size_t) { return Async::LaneID(Core::ComputeCRC16(str), str); }

    /// Default lanes
    namespace Async {
        static constexpr LaneID kDefault = "default"_async;
        static constexpr LaneID kResource = "resource"_async;
    }
}