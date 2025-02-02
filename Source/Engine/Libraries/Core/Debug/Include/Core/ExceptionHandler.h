//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>> Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>> ExceptionHandler.h
//>>
//>>    Exception handling system.  Provides an abstracted thrower for exceptions so
//>>    that exception throwing isn't hardcoded to a block and can be configured.
//>>
//>> (Revision)
//>>    + 10/14/2018 - Nathan Young - Initial creation.
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#pragma once

// Engine debug.
#include "RuntimeException.h"

// C++ STL.
#include <sstream>
#include <iostream>

namespace Djinn {
    namespace Debug {
        class ExceptionHandler {


        public:
            static void Route( RuntimeException const& except ) {
                // For now, just print the exception message to iostream. NCY. 10.14.2018.
                // @TODO: Find a better place to write these to.  LogBuilder?
                std::cout << except.what() << std::endl;
            }
        };
    }
}