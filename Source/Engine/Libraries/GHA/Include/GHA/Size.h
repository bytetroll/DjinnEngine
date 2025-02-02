#pragma once

#include <Core/Size.h>
#include <Core/Hash.h>

namespace Djinn {
    namespace GHA {
        struct Size {
            explicit Size(UInt32 width = 1, UInt32 height = 1, UInt32 depth = 1) : width(width), height(height), depth(depth) {

            }

            USize Hash() {
                return CombineHash(width, height, depth);
            }

            // ...
            UInt32 width;
            UInt32 height;
            UInt32 depth;
        };
    }
}