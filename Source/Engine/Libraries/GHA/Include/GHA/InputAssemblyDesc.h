//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 11/23/2017.
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