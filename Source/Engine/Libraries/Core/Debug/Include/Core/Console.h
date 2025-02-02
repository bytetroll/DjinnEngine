// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

// More to come later (including color text support and console redirect for the editor).

#include <iostream>
#include <string>

namespace Djinn {
    namespace Debug {
        class Console {
        public:
            static void Print( const std::string& Message ) {
                std::cout << Message << std::endl;
            }
        };
    }
}