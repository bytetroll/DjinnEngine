//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <Core/TypeAssignable.h>
#include <GHA/RasterizerStateDesc.h>
#include <GHA/Topology.h>
#include <GHA/MultiSampleStateDesc.h>
#include <GHA/DepthStencilStateDesc.h>
#include <GHA/BlendStateDesc.h>
#include <GHA/InputAssemblyDesc.h>
#include "PipelineBinding.h"
#include "DynamicState.h"

namespace Djinn::GHA {
    class IShader;

    namespace PipelineScheduler {
        struct PipelineDesc {
            PipelineDesc &Shaders(const Core::FixedArray8<IShader *> &value) {
                shaders = value;
                return *this;
            }

            PipelineDesc &AddShader(IShader *value) {
                shaders.Add(value);
                return *this;
            }

            PipelineDesc &InputAssembly(const InputAssemblyDesc &value) {
                inputAssembly = value;
                return *this;
            }

            PipelineDesc &Rasterizer(const RasterizerStateDesc &value) {
                rasterizer = value;
                return *this;
            }

            PipelineDesc &Topology(const Topology &value) {
                topology = value;
                return *this;
            }

            PipelineDesc &DepthStencil(const DepthStencilStateDesc &value) {
                depthStencil = value;
                return *this;
            }

            PipelineDesc &Blend(const BlendStateDesc &value) {
                blend = value;
                return *this;
            }

            PipelineDesc &MultiSample(const MultiSampleStateDesc &value) {
                multiSample = value;
                return *this;
            }

            PipelineDesc &DynamicStates(const Core::FixedArray12<DynamicState> &value) {
                dynamicStates = value;
                return *this;
            }

            PipelineDesc &AddDynamicStates(const DynamicState &value) {
                dynamicStates.Add(value);
                return *this;
            }

            PipelineDesc &ViewportCount(UInt32 viewportCount) {
                this->viewportCount = viewportCount;
                return *this;
            }

            USize Hash() {
                USize hash = 0;
                for (auto shader : shaders) {
                    hash = CombineHash(hash, reinterpret_cast<USize>(shader));
                }
                hash = CombineHash(hash, inputAssembly.Hash(), topology, rasterizer.Hash(), depthStencil.Hash(), blend.Hash(), multiSample.Hash());
                for (auto state : dynamicStates) {
                    hash = CombineHash(hash, state);
                }
                return CombineHash(hash, viewportCount);
            }

            // Shaders to be executed
            Core::FixedArray8<IShader *> shaders;

            // Input Assembly
            InputAssemblyDesc inputAssembly;

            // Topology of input assembly
            GHA::Topology topology = GHA::Topology::eTriangleList;

            // Rasterizer state
            RasterizerStateDesc rasterizer;

            // Depth stencil state
            DepthStencilStateDesc depthStencil;

            // Blend state
            BlendStateDesc blend;

            // Multisample state
            MultiSampleStateDesc multiSample;

            // Dynamic states
            Core::FixedArray12<DynamicState> dynamicStates;

            // Viewport count
            UInt32 viewportCount = 1;
        };
    }
}