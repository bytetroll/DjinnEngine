#pragma once

#include <Core/Types.h>
#include "Rect.h"

namespace Djinn {
    namespace GHA {
        struct Viewport {
            Viewport() : rect(Rect()), minDepth(0), maxDepth(1) {

            }

            Viewport(Rect rect, float minDepth = 0.0f, float maxDepth = 1.0f) : rect(rect), minDepth(minDepth), maxDepth(maxDepth) {

            }

            // ...
            Rect rect;
            float minDepth;
            float maxDepth;
        };
    }
}