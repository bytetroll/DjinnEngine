// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <Core/Common.h>
#include <Core/Types.h>

#include <Core/IAllocator.h>

DJINN_BEGIN_NS2( Djinn, Memory )

template< class AllocatorType >
class IAllocatorFactory {
public:
    DJINN_INLINE static AllocatorType* Create( const Djinn::USize Allotment, IAllocator& Allocator ) {
        const Djinn::USize NewAllocatorSize = Allotment + sizeof( AllocatorType );
        void* const NewAllocator = Allocator.Allocate( NewAllocatorSize, __alignof( AllocatorType ) );
        return new( NewAllocator ) AllocatorType( Allotment, BlockPointerArithmetic::Add( NewAllocator, sizeof( AllocatorType ) ) );
    }

    DJINN_INLINE static void Destroy( IAllocator& Allocator, AllocatorType& Alloc ) {
        Alloc.~AllocatorType();
        Allocator.Deallocate( &Alloc );
    }
};

DJINN_END_NS2( Djinn, Memory )