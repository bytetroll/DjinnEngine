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
 on 11/23/2017.
//
#pragma once

#include <Core/Size.h>

namespace Djinn::GHA {
    struct VertexBufferDesc {
        VertexBufferDesc(USize stride = 0, USize count = 0) : stride(stride), count(count) {

        }

        template<typename T>
        static VertexBufferDesc Make(UInt32 count) {
            return VertexBufferDesc().Stride(sizeof(T)).Count(count);
        }

        VertexBufferDesc& Stride(USize value) {
            stride = value;
            return *this;
        }

        VertexBufferDesc& Count(USize value) {
            count = value;
            return *this;
        }

        VertexBufferDesc& HostVisible(bool value = true) {
            hostVisible = value;
            return *this;
        }

        // Element stride
        USize stride = 16;

        // Number of elements
        USize count = 0;

        // Host visible?
        bool hostVisible = false;
    };
}