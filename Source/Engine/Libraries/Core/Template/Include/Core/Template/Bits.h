#pragma once

#include "Common.h"

namespace Djinn {
    namespace Core {
        // Select type with higher size
        template <class X, class Y>
        using SelectLargest = Select<sizeof(X) >= sizeof(Y), X, Y>;
    }
}