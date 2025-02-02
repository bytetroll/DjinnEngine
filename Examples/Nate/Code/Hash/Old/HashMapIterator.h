// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <Core/Common.h>

#include "HashMapBucket.h"

// @TODO: Change these namespaces after development to Djinn::Core
DJINN_BEGIN_NS2( Djinn, Nate )

// Base class of all hash map iterators.
template< class IterationType >
class HashMapIteratorBase {
public:
    HashMapIteratorBase( const IterationType& Handle ) : Handle( Handle ) {
    }

    bool operator==( const HashMapIteratorBase& Rhs ) {
        return Handle == Rhs.Handle;
    }

    bool operator!=( const HashMapIteratorBase& Rhs ) {
        return Handle != Rhs.Handle;
    }

    HashMapIteratorBase& operator++() {
        ++Handle->Next;
        return* this;
    }

    HashMapIteratorBase& operator--() {
        --Handle->Next;
        return* this;
    }

public:
    IterationType Handle;
};

template< class KeyType, class ValueType, class IterationType = Djinn::Nate::HashMapBucket< KeyType, ValueType >* >
class HashMapIterator : public HashMapIteratorBase< IterationType > {
public:
    HashMapIterator( const IterationType& Iter ) : HashMapIteratorBase< IterationType >( Iter ) {
    }

    IterationType& operator*() {
        //return ( *HashMapIteratorBase< IterationType >::Handle );

        return HashMapIteratorBase< IterationType >::Handle;
    }

    IterationType* operator->() {
        //return &( *HashMapIteratorBase< IterationType >::Handle );
        return *HashMapIteratorBase< IterationType >::Handle;
    }
};

template< class IterationType >
class HashMapConstantIterator : public HashMapIteratorBase< IterationType > {
public:
    HashMapConstantIterator( const IterationType& Iter ) : HashMapIteratorBase< IterationType >( Iter ) {
    }

    const IterationType& operator*() const {
        return *HashMapIteratorBase< IterationType >::Handle;
    }
};

DJINN_END_NS2( Djinn, Nate )