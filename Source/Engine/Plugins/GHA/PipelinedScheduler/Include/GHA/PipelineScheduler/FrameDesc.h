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