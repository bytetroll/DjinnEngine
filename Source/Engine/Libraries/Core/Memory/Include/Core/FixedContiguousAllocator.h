// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/MemoryToolbox.h>
#include <Core/IAllocator.h>
#include <Core/IContiguousAllocator.h>
#include <Core/Assert.h>
#include <Core/Super.h>

DJINN_BEGIN_NS2( Djinn, Memory )

class FixedContiguousAllocator : public IContiguousAllocator {
public:
    FixedContiguousAllocator( const Djinn::USize Allotment, void* const BlockStart ) : IContiguousAllocator( Allotment, BlockStart ) {
    }

    virtual ~FixedContiguousAllocator() {
    }

    virtual void* Allocate( const Djinn::USize Amount, Djinn::UInt8 Alignment = DefaultAlignment ) override {
        DJINN_ASSERT( ( Amount != 0 ) && ( Alignment != 0 ) );

        const Djinn::UInt8 Adjustment = BlockOperations::AlignForwardAdjustment( Super->CurrentBlock, Alignment );

        if( ( ( Super->GetMemoryUsed() + Adjustment ) + Amount ) > Super->GetPoolAllotment() ) {
            return nullptr; // We're out of memory for this allocator.
        }

        //Super->CurrentBlock = nullptr;
        const Djinn::Handle AlignedBlockAddress = reinterpret_cast< Djinn::Handle >( static_cast<UInt8*>(Super->CurrentBlock) + Adjustment );
        Super->CurrentBlock = reinterpret_cast< void* >( AlignedBlockAddress + Amount );
        Super->MemoryUsed = reinterpret_cast< Djinn::Handle >( Super->CurrentBlock ) - reinterpret_cast< Djinn::Handle >( Super->BlockStart );

        return reinterpret_cast< void* >( AlignedBlockAddress );
    }

    virtual void Rewind( void* const ToBlock ) override {
        DJINN_ASSERT( ( Super->CurrentBlock > ToBlock ) && ( Super->BlockStart < ToBlock ) );

        Super->CurrentBlock = ToBlock;
        Super->MemoryUsed = reinterpret_cast< Djinn::Handle >( Super->CurrentBlock ) - reinterpret_cast< Djinn::Handle >( Super->BlockStart );
    }

    void Clear() override final {
        Super->AllocationCount = 0;
        Super->MemoryUsed = 0;

        Super->CurrentBlock = Super->BlockStart;
    }

private:
    FixedContiguousAllocator( const FixedContiguousAllocator& Copy );
    FixedContiguousAllocator& operator=( const FixedContiguousAllocator& Rhs );
};

// @TODO: Convert these to a factory pattern implementation -- NCY
DJINN_INLINE FixedContiguousAllocator* NewFixedContigiousAllocator( const Djinn::USize Allotment, IAllocator& Allocator ) {
    void* const Allocated = Allocator.Allocate( ( Allotment + sizeof( FixedContiguousAllocator ) ), __alignof( FixedContiguousAllocator ) );
    return new( Allocated ) FixedContiguousAllocator( Allotment, BlockPointerArithmetic::Add( Allocated, sizeof( FixedContiguousAllocator ) ) );
}

DJINN_INLINE void DeleteFixedContigiousAllocator( IAllocator& Allocator, FixedContiguousAllocator& FixedAllocator ) {
        FixedAllocator.~FixedContiguousAllocator();
        Allocator.Deallocate( &FixedAllocator );
    }

DJINN_END_NS2( Djinn, Memory )