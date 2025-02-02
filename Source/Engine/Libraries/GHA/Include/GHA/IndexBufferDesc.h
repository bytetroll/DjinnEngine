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
    enum class IndexType {
        eU16 = 2,
        eU32 = 4
    };

    struct IndexBufferDesc {
        IndexBufferDesc(IndexType type = IndexType::eU32, USize count = 0) : type(type), count(count) {

        }

        IndexBufferDesc(USize count) : type(IndexType::eU32), count(count) {

        }

        template<typename T>
        static IndexBufferDesc Make(UInt32 count) {
            if constexpr(Core::IsSame<T, UInt32>::kValue) {
                return IndexBufferDesc().Type(IndexType::eU32).Count(count);
            } else if constexpr (Core::IsSame<T, UInt16>::kValue) {
                return IndexBufferDesc().Type(IndexType::eU16).Count(count);
            } else {
                static_assert("Unsupported index type");
            }
        }

        IndexBufferDesc& Type(IndexType value) {
            type = value;
            return *this;
        }

        IndexBufferDesc& Count(USize value) {
            count = value;
            return *this;
        }

        IndexBufferDesc& HostVisible(bool value = true) {
            hostVisible = value;
            return *this;
        }

        // Element type
        IndexType type;

        // Number of elements
        USize count;

        // Host visible?
        bool hostVisible = false;
    };
}