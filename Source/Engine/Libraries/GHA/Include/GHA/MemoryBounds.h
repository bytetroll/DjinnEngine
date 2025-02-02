#pragma once

#include <Core/Types.h>
#include "AllocationSize.h"

namespace Djinn {
    namespace GHA {
        struct MemoryBounds {
            MemoryBounds() = default;

            MemoryBounds(AllocationSize beginOffset, AllocationSize endOffset) : beginOffset(beginOffset), endOffset(endOffset) {

            }

            MemoryBounds(AllocationSize endOffset) : beginOffset(0), endOffset(endOffset) {

            }

            // Begin Offset
            AllocationSize beginOffset;

            // End Offset
            AllocationSize endOffset;
        };
    }
}