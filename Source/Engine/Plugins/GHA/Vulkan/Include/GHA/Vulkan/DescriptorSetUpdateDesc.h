//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <Core/FixedArray.h>
#include <Core/TypeAssignable.h>
#include "DescriptorBinding.h"

namespace Djinn::GHA::Vulkan {
    struct DescriptorSetUpdateDesc {
        DescriptorSetUpdateDesc& Bindings(const Core::FixedArray128<DescriptorBinding>& value) {
            bindings = value;
            return *this;
        }

        DescriptorSetUpdateDesc& AddBinding(DescriptorBinding value) {
            bindings.Add(value);
            return *this;
        }

        USize Hash() {
            USize hash = 0;
            for (auto layout : bindings) {
                hash = CombineHash(hash, layout.Hash());
            }
            return hash;
        }

        // Bindings
        Core::FixedArray128<DescriptorBinding> bindings;
    };
}