#pragma once

#include <Core/Types.h>
#include "Size.h"

namespace Djinn {
    namespace GHA {
        struct Rect {
            Rect(Size offset, Size extent) : offset(offset), extent(extent) {

            }

            Rect(Size extent = Size()) : offset(0, 0, 0), extent(extent) {

            }

            Rect(UInt32 width, UInt32 height, UInt32 depth = 1) : offset(0, 0, 0), extent(width, height, depth) {

            }

            USize Hash() {
                return CombineHash(offset.Hash(), extent.Hash());
            }

            // ...
            Size offset;
            Size extent;
        };
    }
}