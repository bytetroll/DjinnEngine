// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <Core/Common.h>
#include <Core/Size.h>

#include "KeyValuePair.h"

DJINN_BEGIN_NS2( Djinn, Nate )

template< class KeyType, class ValueType >
struct HashMapNode {
public:
    HashMapNode() : Pair( KeyValuePair< KeyType, ValueType >() ), Hash( 0 ) {
    }

    HashMapNode( const KeyType&& Key, const ValueType&& Value ) :
            Pair( KeyValuePair< KeyType, ValueType >( Key, Value ) ), Hash( 0 ) {
    }

    HashMapNode( const KeyValuePair< KeyType, ValueType >&& Pair ) : Pair( Pair ), Hash( 0 ) {
    }

    HashMapNode( const KeyType&& Key, const ValueType&& Value, const Djinn::UInt32& Hash ) :
            Pair( KeyValuePair< KeyType, ValueType >( Key, Value ) ), Hash( Hash ) {
    }

    HashMapNode( const KeyValuePair< KeyType, ValueType >&& Pair, const Djinn::UInt32& Hash ) :
            Pair( Pair ), Hash( Hash ) {
    }

    ~HashMapNode() {
    }

public:
    KeyValuePair< KeyType, ValueType > Pair;
    Djinn::UInt32 Hash; // This is only set not using a separate hash array.
};

DJINN_END_NS2( Djinn, Nate )