//
// Created by Dev on 11/23/2017.
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