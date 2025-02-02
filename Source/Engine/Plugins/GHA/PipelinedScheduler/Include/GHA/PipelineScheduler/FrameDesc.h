//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <Core/TypeAssignable.h>
#include <Core/FixedArray.h>
#include <GHA/Rect.h>
#include <GHA/ID.h>
#include "ID.h"

namespace Djinn::GHA::PipelineScheduler {
    struct FrameDesc {
        FrameDesc& RenderTargets(const Core::FixedArray16<TextureViewID>& value) {
            renderTargets = value;
            return *this;
        }

        FrameDesc& AddRenderTarget(TextureViewID value) {
            renderTargets.Add(value);
            return *this;
        }

        FrameDesc& RenderArea(const Rect& value) {
            renderArea = value;
            return *this;
        }

        FrameDesc& Layers(UInt32 value) {
            layers = value;
            return *this;
        }

        // Render targets to be written to
        Core::FixedArray16<TextureViewID> renderTargets;

        // Render targets to be written to
        Core::FixedArray16<TextureViewID> resolveRenderTargets;

        // Depth stencil target
        Core::Optional<TextureViewID> depthStencilView;

        // Depth stencil target
        Core::Optional<TextureViewID> resolveDepthStencilView;

        // Area of the render targets to be written to
        Rect renderArea;

        // Number of layers to be written to
        UInt32 layers = 1;
    };
}