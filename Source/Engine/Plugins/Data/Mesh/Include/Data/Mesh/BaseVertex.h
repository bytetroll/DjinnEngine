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
 on 10/23/2017.
//
#pragma once

#include <Math/Vector3.h>
#include <Serialization/Archive.h>
#include <Serialization/Common/VectorMath.h>

namespace Djinn::Data {
    struct BaseVertex {
        BaseVertex& Position(const Math::Vector3& value) {
            position = value;
            return *this;
        }

        BaseVertex& Normal(const Math::Vector3& value) {
            normal = value;
            return *this;
        }

        BaseVertex& Tangent(const Math::Vector3& value) {
            tangent = value;
            return *this;
        }

        BaseVertex& Texcoord(const Math::Vector2& value) {
            texcoord = value;
            return *this;
        }

        Math::Vector3 position;  // 12 : 12 [52]
        Math::Vector3 normal;    // 12 : 24 [40]
        Math::Vector3 tangent;   // 12 : 36 [28]
        Math::Vector2 texcoord;  // 8  : 44 [20]
    };

    /// Serialization helper
    static void operator<<(Serialization::Archive& archive, const BaseVertex& value) {
        archive << value.position;
        archive << value.normal;
        archive << value.tangent;
        archive << value.texcoord;
    }

    /// Serialization helper
    static void operator>>(Serialization::Archive& archive, BaseVertex& value) {
        archive >> value.position;
        archive >> value.normal;
        archive >> value.tangent;
        archive >> value.texcoord;
    }
}