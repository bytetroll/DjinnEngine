#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        enum class Format : Int32;

        // Rate of an input
        enum class InputRate {
            // Data per vertex
                    eVertex,

            // Data per instance
                    eInstance
        };

        // Input description
        struct InputDesc {
            InputDesc &Stride(UInt32 value) {
                stride = value;
                return *this;
            }

            InputDesc &Rate(InputRate value) {
                rate = value;
                return *this;
            }

            USize Hash() {
                return CombineHash(stride, rate);
            }

            // Stride of this input
            UInt32 stride;

            // Rate of this input
            InputRate rate = InputRate::eVertex;
        };
    }
}