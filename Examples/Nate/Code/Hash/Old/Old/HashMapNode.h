// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <Core/Common.h>

#include "Code/Hash/KeyValuePair.h"

DJINN_BEGIN_NS2( Djinn, Nate )

template< class KeyType, class ValueType >
class HashMapNode {
public:
    HashMapNode() : Pair( KeyValuePair< KeyType, ValueType >() ), Next( nullptr ) {
    }

    HashMapNode( const KeyType& Key, const ValueType& Value ) : Pair( KeyValuePair< KeyType, ValueType >() ), Next( nullptr ) {
        Pair = KeyValuePair< KeyType, ValueType >( Key, Value );
    }

    HashMapNode( const KeyValuePair< KeyType, ValueType >& Pair ) : Pair( Pair ), Next( nullptr ) {
    }

    HashMapNode( const KeyType& Key, const ValueType& Value, HashMapNode< KeyType, ValueType >* const Next )
            : Pair( KeyValuePair< KeyType, ValueType >() ), Next( Next ) {
        Pair = KeyValuePair< KeyType, ValueType >( Key, Value );
    }

    HashMapNode( const KeyValuePair< KeyType, ValueType >& Pair, HashMapNode< KeyType, ValueType >* const Next )
        : Pair( Pair ), Next( Next ) {
    }

    ~HashMapNode() {
    }

public:
    KeyValuePair< KeyType, ValueType > Pair;
    HashMapNode< KeyType, ValueType >* Next;
};

DJINN_END_NS2( Djinn, Nate )