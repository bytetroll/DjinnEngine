//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <Core/Array.h>
#include <Core/FixedArray.h>
#include <GHA/PipelineScheduler/DynamicRange.h>

namespace Djinn::GHA::Vulkan {
    class DescriptorSetLayout;

    struct PipelineLayoutDesc {
        PipelineLayoutDesc &Layouts(const Core::Array<DescriptorSetLayout *> &value) {
            layouts = value;
            return *this;
        }

        PipelineLayoutDesc &AddLayout(DescriptorSetLayout *value) {
            layouts.Add(value);
            return *this;
        }

        USize Hash() {
            USize hash = 0;
            for (auto layout : layouts) {
                hash = CombineHash(hash, reinterpret_cast<USize>(layout));
            }
            return hash;
        }

        Core::FixedArray8<PipelineScheduler::DynamicRange> pushConstants;

        // Resource group layouts
        Core::Array<DescriptorSetLayout *> layouts;
    };
}