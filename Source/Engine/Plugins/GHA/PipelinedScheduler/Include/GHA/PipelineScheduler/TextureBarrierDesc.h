//
// Created by Dev on 12/24/2017.
//
#pragma once

#include <GHA/TextureViewDesc.h>
#include <GHA/TextureLayout.h>
#include <GHA/ID.h>
#include "ID.h"

namespace Djinn::GHA::PipelineScheduler {
    using LayoutRange = Bounds<TextureLayout>;

    /// Texture barrier desc
    struct TextureBarrierDesc {
        TextureBarrierDesc& TextureView(TextureViewID value) {
            textureView = value;
            return *this;
        }

        TextureBarrierDesc& Layouts(LayoutRange value) {
            layouts = value;
            return *this;
        }

        TextureBarrierDesc& MipBounds(MipBounds value) {
            mipBounds = value;
            return *this;
        }

        // Target texture view
        TextureViewID textureView;

        // Layout range
        LayoutRange layouts;

        // Mip bounds
        Core::Optional<GHA::MipBounds> mipBounds;
    };
}