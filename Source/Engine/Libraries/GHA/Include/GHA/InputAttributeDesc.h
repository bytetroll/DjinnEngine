#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>
#include <Core/TypeAssignable.h>

namespace Djinn {
    namespace GHA {
        enum class Format : Int32;

        // Input attribute description
        struct InputAttributeDesc {
            InputAttributeDesc &InputIndex(UInt32 value) {
                inputIndex = value;
                return *this;
            }

            InputAttributeDesc &Format(GHA::Format value) {
                format = value;
                return *this;
            }

            USize Hash() {
                return CombineHash(inputIndex, format);
            }

            // Input index
            UInt32 inputIndex = 0;

            // Format of this attribute
            GHA::Format format;
        };
    }
}