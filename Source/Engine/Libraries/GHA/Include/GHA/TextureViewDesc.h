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

#include <Core/Optional.h>
#include <Core/TypeAssignable.h>
#include "Format.h"
#include "TextureViewType.h"
#include "Bounds.h"

namespace Djinn::GHA {
    // Bounds types
    using MipBounds = Bounds<UInt32>;
    using ArrayBounds = Bounds<UInt32>;

    struct TextureViewDesc {
        TextureViewDesc() = default;

        TextureViewDesc(
                GHA::Format format, TextureViewTypeSet viewType, const Core::Optional<GHA::MipBounds>& mipBounds, const Core::Optional<GHA::ArrayBounds>& arrayBounds
        ) : format(format), viewType(viewType), mipBounds(mipBounds), arrayBounds(arrayBounds) {

        }

        TextureViewDesc& Format(GHA::Format value) {
            format = value;
            return *this;
        }

        TextureViewDesc& ViewType(TextureViewTypeSet value) {
            viewType = value;
            return *this;
        }

        TextureViewDesc& MipBounds(MipBounds value) {
            mipBounds = value;
            return *this;
        }

        TextureViewDesc& ArrayBounds(ArrayBounds value) {
            arrayBounds = value;
            return *this;
        }

        USize Hash() {
            return CombineHash(
                    static_cast<USize>(format),
                    static_cast<USize>(viewType.Get()),
                    static_cast<USize>(mipBounds.IsValid()),
                    static_cast<USize>(mipBounds->lower),
                    static_cast<USize>(mipBounds->upper),
                    static_cast<USize>(arrayBounds.IsValid()),
                    static_cast<USize>(arrayBounds->lower),
                    static_cast<USize>(arrayBounds->upper)
            );
        }

        // Mutated format, undefined for automatic
        GHA::Format format = GHA::Format::eUndefined;

        // View type, undefined for automatic
        TextureViewTypeSet viewType = TextureViewType::eUndefined;

        // Mip bounds
        Core::Optional<GHA::MipBounds> mipBounds;

        // Array bounds
        Core::Optional<GHA::ArrayBounds> arrayBounds;
    };

    namespace TextureViews {
        static TextureViewDesc none = {};
    }
}