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
 on 12/22/2017.
//
#pragma once

#include <Core/Unique.h>
#include "Rect.h"
#include "Viewport.h"
#include "TextureViewDesc.h"
#include "SamplerDesc.h"

namespace Djinn {
    namespace GHA {
        // All Number IDs use the Resource ID
        DJINN_UNIQUEID(ResourceID, Int32);

        // Number IDs
        DJINN_UNIQUEID(TextureID, ResourceID);
        DJINN_UNIQUEID(SamplerID, ResourceID);
        DJINN_UNIQUEID(StructuredBufferID, ResourceID);
        DJINN_UNIQUEID(TextureBufferID, ResourceID);
        DJINN_UNIQUEID(VertexBufferID, ResourceID);
        DJINN_UNIQUEID(IndexBufferID, ResourceID);
        DJINN_UNIQUEID(ConstantBufferID, ResourceID);
        DJINN_UNIQUEID(FenceID, ResourceID);
        DJINN_UNIQUEID(PassID, ResourceID);
        DJINN_UNIQUEID(FrameID, ResourceID);

        // Bind IDs
        DJINN_UNIQUEID(BindID, UInt64);
    }

    /// Common hashes
    template<> struct Hasher<GHA::BindID > { USize operator()(const GHA::BindID& value) const { return Hasher<Int64>{}(value.Get()); }};
    template<> struct Hasher<GHA::ResourceID> { USize operator()(const GHA::ResourceID& value) const { return Hasher<Int64>{}(value.Get()); }};
    template<> struct Hasher<GHA::TextureID> { USize operator()(const GHA::TextureID& value) const { return Hasher<Int64>{}(value.Get()); }};
    template<> struct Hasher<GHA::SamplerID> { USize operator()(const GHA::SamplerID& value) const { return Hasher<Int64>{}(value.Get()); }};
    template<> struct Hasher<GHA::StructuredBufferID> { USize operator()(const GHA::StructuredBufferID& value) const { return Hasher<Int64>{}(value.Get()); }};
    template<> struct Hasher<GHA::TextureBufferID> { USize operator()(const GHA::TextureBufferID& value) const { return Hasher<Int64>{}(value.Get()); }};
    template<> struct Hasher<GHA::VertexBufferID> { USize operator()(const GHA::VertexBufferID& value) const { return Hasher<Int64>{}(value.Get()); }};
    template<> struct Hasher<GHA::IndexBufferID> { USize operator()(const GHA::IndexBufferID& value) const { return Hasher<Int64>{}(value.Get()); }};
    template<> struct Hasher<GHA::FenceID> { USize operator()(const GHA::FenceID& value) const { return Hasher<Int64>{}(value.Get()); }};
}