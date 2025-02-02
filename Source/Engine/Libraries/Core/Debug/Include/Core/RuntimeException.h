// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

// Example
//
// throw std::runtime_error( Formatter() << foo << 13 << ", bar" << myData );   Implicitly cast to std::string
// throw std::runtime_error( Formatter() << foo << 13 << ", bar" << myData >> Formatter::to_str );    Explicitly cast to std::string

#include <sstream>

namespace Djinn {
    namespace Debug {
        using RuntimeException = std::runtime_error;
    }
}