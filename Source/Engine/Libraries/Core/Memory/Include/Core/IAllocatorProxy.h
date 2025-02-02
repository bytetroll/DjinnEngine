// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <new>
#include <functional>

#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/MemoryToolbox.h>
#include <Core/IAllocator.h>
#include <Core/Assert.h>
#include <Core/Super.h>

// Gateway for allocators.  Allows tracking of allocations.  Used by the profiler.

DJINN_BEGIN_NS2( Djinn, Memory )

class IAllocatorProxy : public IAllocator {
public:
    IAllocatorProxy( IAllocator& Allocator ) : IAllocator( Allocator.GetPoolAllotment() ), Allocator( Allocator ) {
    }

    ~IAllocatorProxy() {
    }

    void* Allocate( const Djinn::USize Amount, Djinn::UInt8 Alignment ) override {
        DJINN_ASSERT( Amount != 0 );

        ++AllocationCount;

        const Djinn::USize UsedMemory = Allocator.GetMemoryUsed();
        void* const Allocation = Allocator.Allocate( Amount, Alignment );
        Super->MemoryUsed += Allocator.GetMemoryUsed() - UsedMemory;

        return Allocation;
    }

    void Deallocate( void* const Block ) override {
        --AllocationCount;

        const Djinn::USize UsedMemory = Allocator.GetMemoryUsed();
        Allocator.Deallocate( Block );
        Super->MemoryUsed -= UsedMemory - Allocator.GetMemoryUsed();
    }

    IAllocator* GetAllocator() const {
        return &Allocator;
    }

private:
    IAllocatorProxy( const IAllocatorProxy& Copy );
    IAllocatorProxy& operator=( const IAllocatorProxy& Rhs );

private:
    IAllocator& Allocator;
};

DJINN_END_NS2( Djinn, Memory )