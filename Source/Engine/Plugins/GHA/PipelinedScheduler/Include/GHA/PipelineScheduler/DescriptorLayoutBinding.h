//
// Created by Dev on 2/2/2018.
//
#pragma once

#include <Core/Types.h>
#include <COM/ID.h>
#include <GHA/Stage.h>
#include "DescriptorType.h"

namespace Djinn::GHA::PipelineScheduler {
    struct DescriptorLayoutBinding {
        DescriptorLayoutBinding &Class(DescriptorType value) {
            id = value;
            return *this;
        }

        DescriptorLayoutBinding &Binding(UInt32 value) {
            binding = value;
            return *this;
        }

        DescriptorLayoutBinding &Stages(const GHA::StageSet& value) {
            stages = value;
            return *this;
        }

        DescriptorLayoutBinding &AddStage(const GHA::StageSet& value) {
            stages += value;
            return *this;
        }

        DescriptorLayoutBinding &Count(UInt32 value) {
            count = value;
            return *this;
        }

        USize Hash() {
            return CombineHash(binding, id, stages.Get(), count);
        }

        // Binding index
        UInt32 binding = 0;

        // Class of resource
        DescriptorType id;

        // Stages this descriptor is visible to
        GHA::StageSet stages;

        // Array count
        UInt32 count = 0;
    };
}