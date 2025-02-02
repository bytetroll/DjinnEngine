//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <Core/TypeAssignable.h>
#include <Core/FixedArray.h>
#include <GHA/Rect.h>
#include <GHA/ID.h>

namespace Djinn::GHA::Vulkan {
    class TextureView;

    struct FrameDesc {
        FrameDesc& ResolveRenderTargets(const Core::FixedArray16<TextureView*>& value) {
            resolveRenderTargets = value;
            return *this;
        }

        FrameDesc& RenderTargets(const Core::FixedArray16<TextureView*>& value) {
            renderTargets = value;
            return *this;
        }

        FrameDesc& AddRenderTarget(TextureView* value) {
            renderTargets.Add(value);
            return *this;
        }

        FrameDesc& AddResolveRenderTarget(TextureView* value) {
            resolveRenderTargets.Add(value);
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

        USize Hash() {
            USize hash = 0;
            for (auto target : renderTargets) {
                hash = CombineHash(hash, target);
            }
            for (auto target : resolveRenderTargets) {
                hash = CombineHash(hash, target);
            }
            return CombineHash(hash, renderArea.Hash(), layers);
        }

        // Render targets to be written to
        Core::FixedArray16<TextureView*> renderTargets;

        // Resolve render targets
        Core::FixedArray16<TextureView*> resolveRenderTargets;

        // Area of the render targets to be written to
        Rect renderArea;

        // Number of layers to be written to
        UInt32 layers = 1;
    };
}