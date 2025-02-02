//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 11/27/2017.
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