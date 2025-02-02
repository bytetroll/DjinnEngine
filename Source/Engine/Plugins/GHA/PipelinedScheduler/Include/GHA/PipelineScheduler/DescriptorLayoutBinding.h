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
 on 2/2/2018.
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