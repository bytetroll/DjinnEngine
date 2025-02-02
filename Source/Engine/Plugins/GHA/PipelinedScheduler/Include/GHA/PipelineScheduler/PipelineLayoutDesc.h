//
// Created by Dev on 2/3/2018.
//
#pragma once

#include <Core/FixedArray.h>
#include <GHA/ID.h>
#include "DynamicRange.h"

namespace Djinn::GHA::PipelineScheduler {
    struct PipelineLayoutDesc {
        PipelineLayoutDesc& AddLayout(ResourceID id) {
            layouts.Add(id);
            return *this;
        }

        Core::FixedArray8<DynamicRange> dynamicRanges;

        Core::FixedArray8<ResourceID> layouts;
    };
}