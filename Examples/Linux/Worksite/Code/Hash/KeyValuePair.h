// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <Core/Common.h>

DJINN_BEGIN_NS2( Djinn, Nate )

template< class KeyType, class ValueType >
class KeyValuePair {
public:
    KeyValuePair() {
    }

    KeyValuePair( const KeyType& Key, const ValueType& Value ) : Key( Key ), Value( Value ) {
    }

public:
    KeyType Key;
    ValueType Value;
};

DJINN_END_NS2( Djinn, Nate )