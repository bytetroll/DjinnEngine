// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <Core/Common.h>

DJINN_BEGIN_NS2( Djinn, Nate )

template< class KeyType, class ValueType >
struct HashMapBucket {
public:
    HashMapBucket( const KeyType& Key, const ValueType& Value ) : Key( Key ), Value( Value ), Next( nullptr ) {
    }

    HashMapBucket( const KeyType& Key, const ValueType& value, HashMapBucket* const Next ) : Key( Key ), Value( Value ),
        Next( Next ) {
    }

    bool operator!=( const HashMapBucket< KeyType, ValueType >& Rhs ) {
        return ( Key != Rhs.Key ) || ( Value != Rhs.Value );
    }

    bool operator==( const HashMapBucket< KeyType, ValueType >& Rhs ) {
        return ( Key == Rhs.Key ) && ( Value == Rhs.Value );
    }

public:
    KeyType Key;
    ValueType Value;

    HashMapBucket* Next; // Next entry with the same node for collision chaining.
};

DJINN_END_NS2( Djinn, Nate )