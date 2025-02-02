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

#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Serialization/Archive.h>

namespace Djinn::Math {
    /// Serialization helper
    static void operator<<(Serialization::Archive &archive, const Vector2 &vec) {
        archive << vec.x << vec.y;
    }

    /// Serialization helper
    static void operator>>(Serialization::Archive &archive, Vector2 &out) {
        archive >> out.x >> out.y;
    }

    /// Serialization helper
    static void operator<<(Serialization::Archive &archive, const Vector3 &vec) {
        archive << vec.x << vec.y << vec.z;
    }

    /// Serialization helper
    static void operator>>(Serialization::Archive &archive, Vector3 &out) {
        archive >> out.x >> out.y >> out.z;
    }

    /// Serialization helper
    static void operator<<(Serialization::Archive &archive, const Vector4 &vec) {
        archive << vec.x << vec.y << vec.z << vec.w;
    }

    /// Serialization helper
    static void operator>>(Serialization::Archive &archive, Vector4 &out) {
        archive >> out.x >> out.y >> out.z >> out.w;
    }
}