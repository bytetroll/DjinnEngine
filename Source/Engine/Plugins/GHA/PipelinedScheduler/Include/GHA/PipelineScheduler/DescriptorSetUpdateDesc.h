//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <Core/FixedArray.h>
#include <Core/TypeAssignable.h>
#include "DescriptorBinding.h"

namespace Djinn::GHA::PipelineScheduler {
    struct DescriptorSetUpdateDesc {
        DescriptorSetUpdateDesc& Bindings(const Core::FixedArray128<DescriptorBinding>& value) {
            bindings = value;
            return *this;
        }

        DescriptorSetUpdateDesc& AddBinding(DescriptorBinding value) {
            bindings.Add(value);
            return *this;
        }

        // Bindings
        Core::FixedArray128<DescriptorBinding> bindings;
    };
}