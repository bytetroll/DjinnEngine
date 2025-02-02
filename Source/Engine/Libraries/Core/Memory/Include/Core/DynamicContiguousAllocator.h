// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/Super.h>
#include <Core/Assert.h>
#include <Core/IContiguousAllocator.h>

DJINN_BEGIN_NS2( Djinn, Memory )

class DynamicContiguousAllocator : public IContiguousAllocator  {
public:
    struct ContiguousBlockDescriptor {
    public:
        ContiguousBlockDescriptor() : PreviousBlock( nullptr ), BlockSize( 0 ),
            MemoryUsed( 0 ) {
        }

    private:
        ContiguousBlockDescriptor( const ContiguousBlockDescriptor& Copy ) {
            // No copy.
        }

        ContiguousBlockDescriptor& operator=( const ContiguousBlockDescriptor& Rhs ) {
            // No assign.
        }

    public:
        void* Start;
        ContiguousBlockDescriptor* PreviousBlock;
        Djinn::USize BlockSize;
        Djinn::USize MemoryUsed;
    };

public:
    DynamicContiguousAllocator( IAllocator& Allocator, const Djinn::USize BlockSize, Djinn::UInt8 BlockAlignment ) :
        IContiguousAllocator( 1, nullptr ), Allocator( Allocator ), CurrentBlockInfo( nullptr ), BlockSize( BlockSize ),
        BlockAlignment( BlockAlignment ) {
        // This is a crappy way to deal with the fact that we need the dynamic contiguous allocator to start at 0
        // despite the fact that for other allocators the pool allotment cannot be 0.
        Super->PoolAllotment = 0;
    }

    ~DynamicContiguousAllocator() override final {
    }

    void* const Allocate( const Djinn::USize Amount, Djinn::UInt8 Alignment = DefaultAlignment ) override final {
        DJINN_ASSERT( ( Amount != 0 ) && ( Alignment != 0 ) );

        if( CurrentBlock == nullptr ) {
            AllocateNewBlock( Amount, Alignment );
        }

        Djinn::UInt8 Adjustment = BlockOperations::AlignForwardAdjustment( Super->CurrentBlock, Alignment );
        if( ( ( CurrentBlockInfo->MemoryUsed + Adjustment ) + Amount ) > CurrentBlockInfo->BlockSize ) {
            AllocateNewBlock( Amount, Alignment );
            Adjustment = BlockOperations::AlignForwardAdjustment( Super->CurrentBlock, Alignment );
        }

        void* const AlignedAddress = BlockPointerArithmetic::Add( Super->CurrentBlock, Adjustment );
        const Djinn::USize TotalAdjustedSize = Adjustment + Amount;

        Super->CurrentBlock = BlockPointerArithmetic::Add( Super->CurrentBlock, TotalAdjustedSize );
        Super->MemoryUsed += TotalAdjustedSize;

        CurrentBlockInfo->MemoryUsed += TotalAdjustedSize;

        return AlignedAddress;
    }

    void Rewind( void* const ToBlock ) override final {
        if( ToBlock == Super->CurrentBlock ) {
            // We don't want to rewind to the current block. -- NCY
            return;
        }

        DJINN_ASSERT( CurrentBlockInfo != nullptr );

        while( ( ToBlock <= ( Super->CurrentBlockInfo + 1 ) ) ||
                ( ToBlock > BlockPointerArithmetic::Add( Super->CurrentBlockInfo, Super->CurrentBlockInfo->BlockSize ) ) ) {
            Super->PoolAllotment -= CurrentBlockInfo->BlockSize;
            Super->MemoryUsed -= CurrentBlockInfo->MemoryUsed;

            ContiguousBlockDescriptor* const PreviousBlock = CurrentBlockInfo->PreviousBlock;

            Allocator.Deallocate( CurrentBlockInfo->Start );
            CurrentBlockInfo = PreviousBlock;

            if( CurrentBlock != nullptr ) {
                Super->MemoryUsed -= CurrentBlockInfo->BlockSize - CurrentBlockInfo->MemoryUsed;
            } else {
                DJINN_ASSERT( ToBlock == nullptr );
                Super->CurrentBlock = nullptr;
                return;
            }
        }

        DJINN_ASSERT( ToBlock != nullptr );
        DJINN_ASSERT( ( ToBlock > ( CurrentBlockInfo + 1) ) && ( ToBlock <= ( BlockPointerArithmetic::Add( CurrentBlockInfo->Start,
            CurrentBlockInfo->MemoryUsed ) ) ) );

        const Djinn::USize UsedMemory = CurrentBlockInfo->MemoryUsed - ( reinterpret_cast< Djinn::Handle >( ToBlock ) -
            reinterpret_cast< Djinn::Handle >( CurrentBlockInfo->Start ) );
        CurrentBlockInfo->MemoryUsed -= UsedMemory;
        Super->MemoryUsed -= UsedMemory;
        Super->CurrentBlock = ToBlock;
    }

    void Clear() override final {
        Rewind( nullptr );
        DJINN_ASSERT( ( Super->MemoryUsed == 0 ) && ( Super->PoolAllotment == 0 ) && ( Super->AllocationCount == 0 ) &&
                        ( Super->BlockStart == nullptr ) && ( Super->CurrentBlock == nullptr ) && ( CurrentBlockInfo == nullptr ) );
    }

private:
    DynamicContiguousAllocator( const DynamicContiguousAllocator& Copy );
    DynamicContiguousAllocator& operator=( const DynamicContiguousAllocator& Rhs );

    bool AllocateNewBlock( const Djinn::USize Amount, Djinn::UInt8 Alignment ) {
        const Djinn::USize TotalAllocationSize = ( Amount + sizeof( ContiguousBlockDescriptor ) ) + __alignof( ContiguousBlockDescriptor );
        Djinn::USize NumberOfBlocks = TotalAllocationSize / BlockSize;

        if( ( TotalAllocationSize % BlockSize ) == 0 ) {
            ++NumberOfBlocks;
        }

        const Djinn::USize NewBlockSize = NumberOfBlocks * BlockSize;

        void* const NewBlock = Allocator.Allocate( NewBlockSize, BlockAlignment );
        if( NewBlock == nullptr ) {
            return false;
        }

        const Djinn::UInt8 NewBlockAdjustment = BlockOperations::AlignForwardAdjustment( NewBlock, __alignof( ContiguousBlockDescriptor ) );
        ContiguousBlockDescriptor* const NewBlockDescriptor = static_cast< ContiguousBlockDescriptor* >(
                BlockPointerArithmetic::Add( NewBlock, NewBlockAdjustment )
        );
        NewBlockDescriptor->Start = NewBlock;
        NewBlockDescriptor->PreviousBlock = CurrentBlockInfo;
        NewBlockDescriptor->BlockSize = NewBlockSize;
        NewBlockDescriptor->MemoryUsed = sizeof( ContiguousBlockDescriptor ) + NewBlockAdjustment;

        if( CurrentBlockInfo != nullptr ) {
            Super->MemoryUsed += CurrentBlockInfo->BlockSize - CurrentBlockInfo->MemoryUsed;
        }

        Super->MemoryUsed += NewBlockDescriptor->MemoryUsed;
        CurrentBlockInfo = NewBlockDescriptor;
        Super->CurrentBlock = CurrentBlockInfo + 1;
        Super->PoolAllotment += NewBlockSize;

        return true;
    }

private:
    IAllocator& Allocator;
    ContiguousBlockDescriptor* CurrentBlockInfo;

    Djinn::USize BlockSize;
    Djinn::UInt8 BlockAlignment;
};


DJINN_END_NS2( Djinn, Memory )