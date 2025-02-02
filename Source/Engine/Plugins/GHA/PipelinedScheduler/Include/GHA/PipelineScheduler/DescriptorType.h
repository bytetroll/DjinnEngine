//
// Created by Dev on 2/3/2018.
//
#pragma once

namespace Djinn::GHA::PipelineScheduler {
    enum class DescriptorType {
        eNone,
        eConstantBuffer,
        eTextureView,
        eUnorderedTextureView,
        eUnorderedVertexBufferView,
        eUnorderedIndexBufferView,
        eStructuredBufferView,
        eUnorderedStructuredBufferView,
        eTextureBufferView,
        eUnorderedTextureBufferView,
    };
}