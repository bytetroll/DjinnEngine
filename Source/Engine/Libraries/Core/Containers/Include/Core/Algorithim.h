// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <Core/Types.h>
#include "Array.h"

#include <iostream>

namespace Djinn {
    namespace Core {
        template< typename ElementType, typename Iterator = typename Array< ElementType >::Iterator >
        void Reverse( Iterator Start, Iterator End ) {
            --End; // For what ever reason, end iterator points one block past scope.
            while( &*Start <= &*End ) {
                ElementType Temp = *Start;
                *Start = *End;
                *End = Temp;
                ++Start;
                --End;
            }
        }
    }
}