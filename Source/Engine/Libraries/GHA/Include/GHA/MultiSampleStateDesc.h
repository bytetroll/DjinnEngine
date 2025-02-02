#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>
#include <Core/Hash.h>
#include "SampleMode.h"

namespace Djinn {
    namespace GHA {
        struct MultiSampleStateDesc {
            MultiSampleStateDesc& SampleMode(GHA::SampleMode value) {
                sampleMode = value;
                return *this;
            }

            MultiSampleStateDesc& AlphaCoverage(bool value) {
                alphaCoverage = value;
                return *this;
            }

            MultiSampleStateDesc& AlphaOne(bool value) {
                alphaOne = value;
                return *this;
            }

            USize Hash() {
                return CombineHash(sampleMode, alphaCoverage, alphaOne);
            }

            // Sample mode
            GHA::SampleMode sampleMode = GHA::SampleMode::e1;

            // Alpha coverage support
            bool alphaCoverage = false;

            // Alpha one support
            bool alphaOne = false;
        };
    }
}