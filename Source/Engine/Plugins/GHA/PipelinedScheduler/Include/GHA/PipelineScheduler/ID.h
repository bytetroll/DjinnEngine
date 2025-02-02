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
#include <GHA/ID.h>

namespace Djinn {
    namespace GHA::PipelineScheduler {
        // Number IDs
        DJINN_UNIQUEID(TextureViewID, ResourceID);
        DJINN_UNIQUEID(PersistentID, ResourceID);
        DJINN_UNIQUEID(TextureUAVID, ResourceID);
    }

    /// Common hashes
    template<> struct Hasher<GHA::PipelineScheduler::TextureViewID> { USize operator()(const GHA::PipelineScheduler::TextureViewID& value) const { return Hasher<Int64>{}(value.Get()); }};
    template<> struct Hasher<GHA::PipelineScheduler::TextureUAVID> { USize operator()(const GHA::PipelineScheduler::TextureUAVID& value) const { return Hasher<Int64>{}(value.Get()); }};
}