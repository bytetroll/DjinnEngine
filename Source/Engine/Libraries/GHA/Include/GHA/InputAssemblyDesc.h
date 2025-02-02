//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <Core/TypeAssignable.h>
#include <Core/FixedArray.h>
#include "InputDesc.h"
#include "InputAttributeDesc.h"

namespace Djinn::GHA {
    struct InputAssemblyDesc {
        InputAssemblyDesc& Inputs(const Core::FixedArray12<InputDesc>& value) {
            inputs = value;
            return *this;
        }

        InputAssemblyDesc& AddInput(const InputDesc& value) {
            inputs.Add(value);
            return *this;
        }

        InputAssemblyDesc& InputAttributes(const Core::FixedArray32<InputAttributeDesc>& value) {
            inputAttributes = value;
            return *this;
        }

        InputAssemblyDesc& AddInputAttribute(const InputAttributeDesc& value) {
            inputAttributes.Add(value);
            return *this;
        }

        USize Hash() {
            USize hash = 0;
            for (auto input : inputs) {
                hash = CombineHash(hash, input.Hash());
            }
            for (auto attr : inputAttributes) {
                hash = CombineHash(hash, attr.Hash());
            }
            return hash;
        }

        // Inputs
        Core::FixedArray12<InputDesc> inputs;

        // Input Attributes
        Core::FixedArray32<InputAttributeDesc> inputAttributes;
    };
}