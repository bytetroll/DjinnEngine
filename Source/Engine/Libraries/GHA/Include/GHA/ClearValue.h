//
// Created by Dev on 11/27/2017.
//
#pragma once

#include <Core/Types.h>

namespace Djinn::GHA {
    union ColorClearValue {
        static ColorClearValue F32(float x, float y, float z, float w) {
            ColorClearValue value;
            value.f32[0] = x;
            value.f32[1] = y;
            value.f32[2] = z;
            value.f32[3] = w;
            return value;
        }

        static ColorClearValue I32(Int32 x, Int32 y, Int32 z, Int32 w) {
            ColorClearValue value;
            value.i32[0] = x;
            value.i32[1] = y;
            value.i32[2] = z;
            value.i32[3] = w;
            return value;
        }

        static ColorClearValue U32(UInt32 x, UInt32 y, UInt32 z, UInt32 w) {
            ColorClearValue value;
            value.u32[0] = x;
            value.u32[1] = y;
            value.u32[2] = z;
            value.u32[3] = w;
            return value;
        }

        float f32[4];
        Int32 i32[4];
        UInt32 u32[4];
    };

    struct StencilClearValue {
        float depth;
        UInt8 stencil;
    };
}