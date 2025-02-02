// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <string>

#include <Core/Common.h>


DJINN_BEGIN_NS2( Djinn, Nate )

namespace StringifiableBase {
    using std::to_string;

    template< class Type >
    std::string AsString( Type&& Object ) {
        return to_string( std::forward< Type >( Object ) );
    }
}

template< class Type >
std::string ToString( Type&& Object ) {
    return StringifiableBase::AsString( std::forward< Type >( Object ) );
}

#define DJINN_TO_STRING( CLASS ) friend std::string to_string( CLASS const& Self )

DJINN_END_NS2( Djinn, Nate )