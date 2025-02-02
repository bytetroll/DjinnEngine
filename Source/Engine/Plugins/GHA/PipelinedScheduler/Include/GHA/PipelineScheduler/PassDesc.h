//
// Created by Dev on 11/23/2017.
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