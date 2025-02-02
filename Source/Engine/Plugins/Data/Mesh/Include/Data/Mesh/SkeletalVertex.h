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

#include "BaseVertex.h"

namespace Djinn::Data {
    struct SkeletalVertex : public BaseVertex {
        static_assert(sizeof(BaseVertex) == 44, "Unexpected base vertex size");
        Math::Vector4 boneIndices;  // 16  : 60 [4]
        Math::Vector4 boneWeights;  // 16  : 76 [-12]
    };

    /// Serialization helper
    static void operator<<(Serialization::Archive& archive, const SkeletalVertex& value) {
        archive << static_cast<const BaseVertex&>(value);
        archive << value.boneIndices;
        archive << value.boneWeights;
    }

    /// Serialization helper
    static void operator>>(Serialization::Archive& archive, SkeletalVertex& value) {
        archive >> static_cast<BaseVertex&>(value);
        archive >> value.boneIndices;
        archive >> value.boneWeights;
    }
}