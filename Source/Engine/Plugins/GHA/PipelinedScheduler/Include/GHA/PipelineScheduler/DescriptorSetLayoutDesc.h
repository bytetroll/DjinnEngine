//
// Created by Dev on 2/2/2018.
//
#pragma once

#include <Core/FixedArray.h>
#include "DescriptorLayoutBinding.h"

namespace Djinn::GHA::PipelineScheduler {
    struct DescriptorSetLayoutDesc {
        DescriptorSetLayoutDesc &AddBinding(const DescriptorLayoutBinding &binding) {
            bindings.Add(binding);
            return *this;
        }

        USize Hash() {
            USize hash = 0;
            for (auto& bind : bindings) {
                hash = CombineHash(hash, bind.Hash());
            }
            return hash;
        }

        Core::FixedArray64<DescriptorLayoutBinding> bindings;
    };
}