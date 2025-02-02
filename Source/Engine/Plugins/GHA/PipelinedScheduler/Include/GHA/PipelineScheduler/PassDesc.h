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
#include <Core/FixedArray.h>
#include <Core/TypeAssignable.h>
#include <GHA/SampleMode.h>
#include <GHA/RenderTargetDesc.h>

namespace Djinn::GHA::PipelineScheduler {
    struct PassDesc {
        PassDesc& DependencyCount(UInt32 value) {
            dependencyCount = value;
            return *this;
        }

        PassDesc& ColorRenderTargets(const Core::FixedArray12<RenderTargetDesc>& value) {
            colorRenderTargets = value;
            return *this;
        }

        PassDesc& AddColorRenderTarget(const RenderTargetDesc& value) {
            colorRenderTargets.Add(value);
            return *this;
        }

        PassDesc& DepthStencilTarget(const RenderTargetDesc& value) {
            depthStencilTarget = value;
            return *this;
        }

        USize Hash() {
            USize hash = Djinn::Hash(dependencyCount);
            for (auto& target : colorRenderTargets) {
                hash = CombineHash(hash, target.Hash());
            }
            if (depthStencilTarget.IsValid()) {
                hash = CombineHash(hash, depthStencilTarget->Hash());
            }
            for (auto& target : colorResolveRenderTargets) {
                hash = CombineHash(hash, target.Hash());
            }
            if (depthResolveStencilTarget.IsValid()) {
                hash = CombineHash(hash, depthResolveStencilTarget->Hash());
            }
            return CombineHash(hash);
        }

        // External passes this pass depends on
        UInt32 dependencyCount = 0;

        // Color render target descriptions
        Core::FixedArray12<RenderTargetDesc> colorRenderTargets;

        // Color resolve render target descriptions
        Core::FixedArray12<RenderTargetDesc> colorResolveRenderTargets;

        // Depth Stencil render target description
        Core::Optional<RenderTargetDesc> depthStencilTarget;

        // Depth Resolve Stencil render target description
        Core::Optional<RenderTargetDesc> depthResolveStencilTarget;
    };
}