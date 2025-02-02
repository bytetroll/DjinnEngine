// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <new>
#include <functional>

#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/MemoryToolbox.h>
#include <Core/Assert.h>

DJINN_BEGIN_NS2( Djinn, Memory )

class IAllocator {
public:
    static constexpr Djinn::UInt8 DefaultAlignment = 8;

public:
    IAllocator( const Djinn::USize PoolAllotment ) : PoolAllotment( PoolAllotment ), MemoryUsed( 0 ),
        AllocationCount( 0 ) {
        DJINN_ASSERT( PoolAllotment > 0);
    }

    virtual ~IAllocator() {
        // There is a lot of room for leak detection here.  For now just using asserts -- NCY.
        DJINN_ASSERT( ( AllocationCount == 0 ) && ( MemoryUsed == 0 ) );
    }

    virtual void* Allocate( const Djinn::USize Amount, Djinn::UInt8 Alignment = DefaultAlignment ) = 0;
    virtual void Deallocate( void* const Block ) = 0;

    Djinn::USize GetPoolAllotment() const {
        return PoolAllotment;
    }

    Djinn::USize GetMemoryUsed() const {
        return MemoryUsed;
    }

    Djinn::USize GetAllocationCount() const {
        return AllocationCount;
    }

protected:
    IAllocator( const IAllocator& Copy );
    IAllocator& operator=( const IAllocator& Rhs );

protected:
    Djinn::USize PoolAllotment;
    Djinn::USize MemoryUsed;
    Djinn::USize AllocationCount;
};


DJINN_END_NS2( Djinn, Memory )