//
// Created by Dev on 5/5/2018.
//
#pragma once

#include <Core/Array.h>
#include "Token.h"

namespace Djinn::Data::UIML {
    struct TokenBlob {
        Core::Array<Token> tokens;
    };
}
