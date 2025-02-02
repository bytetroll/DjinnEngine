// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/MemoryToolbox.h>
#include <Core/IAllocator.h>
#include <Core/Assert.h>
#include <Core/Super.h>

// Free list allocator is primarily used as the main allocator -- i.e., that manages the allocated pool.
// This allocator implements a typical free list algorithm often used for heap management.  All blocks
// are the same size.

DJINN_BEGIN_NS2( Djinn, Memory )

class FreeListAllocator : public IAllocator {
private:
    static constexpr Djinn::UInt32 AllocationMagicNumber = 0xAEF01762;

    struct AllocationHeader {
    public:
        AllocationHeader() : Size( 0 ), Adjustment( 0 ) {
            #if DJINN_DEBUG
                Identifier = 0;
                MagicNumber = 0;
            #endif
        }

    public:
        Djinn::USize Size;
        Djinn::UInt8 Adjustment;

        #if DJINN_DEBUG
            Djinn::USize Identifier;
            Djinn::UInt32 MagicNumber;
        #endif
    };

    struct FreeBlock {
    public:
        FreeBlock() : Size( 0 ), Next( nullptr ) {
            #if DJINN_DEBUG
                MagicNumber = 0;
            #endif
        }

    public:
        Djinn::USize Size;
        FreeBlock* Next;

        #if DJINN_DEBUG
            Djinn::UInt32 MagicNumber;
        #endif
    };

    static_assert( sizeof( AllocationHeader ) >= sizeof( FreeBlock ), "Sizeof allocation header is less than size of free block." );

public:
    FreeListAllocator( const Djinn::USize Allotment, void* const BlockStart ) : IAllocator( Allotment ),
        FreeBlocks( static_cast< FreeBlock* >( BlockStart ) ) {
        DJINN_ASSERT( Allotment > sizeof( FreeBlock ) );

        FreeBlocks->Size = Allotment;
        FreeBlocks->Next = nullptr;

        #if DJINN_DEBUG
            FreeBlocks->MagicNumber = AllocationMagicNumber;
            NextIdentifier = 1;
        #endif
    }

    ~FreeListAllocator() {
        //if( FreeBlocks != nullptr ) {
        //    FreeBlocks = nullptr;
        //}
    }

    void* Allocate( const Djinn::USize Amount, Djinn::UInt8 Alignment ) override {
        DJINN_ASSERT( ( Amount != 0 ) && ( Alignment != 0 ) );

        FreeBlock* PreviousFreeBlock = nullptr;
        FreeBlock* CurrentFreeBlock = FreeBlocks;

        FreeBlock* BestFitPrevious = nullptr;
        FreeBlock* BestFitCurrent = nullptr;
        Djinn::UInt8 BestFitAdjustment = 0;
        Djinn::USize BestFitTotalSize = 0;

        // Find the best fit for our current block.
        while( CurrentFreeBlock != nullptr ) {
#if DJINN_DEBUG
            DJINN_ASSERT( CurrentFreeBlock->MagicNumber == AllocationMagicNumber );
#endif

            // Calculate the adjustment needed to keep object correctly aligned.
            const Djinn::UInt8 AdjustmentNeeded = BlockOperations::AlignForwardAdjustmentWithHeader< AllocationHeader >(
                    CurrentFreeBlock, Alignment
            );
            Djinn::USize NeededTotalSize = Amount + AdjustmentNeeded;

            // If it fits, use it.
            if( CurrentFreeBlock->Size == NeededTotalSize ) {
                BestFitPrevious = PreviousFreeBlock;
                BestFitCurrent = CurrentFreeBlock;
                BestFitAdjustment = AdjustmentNeeded;
                BestFitTotalSize = NeededTotalSize;

                break;
            }

            // If there's a better fit, switch it.
            if( ( CurrentFreeBlock->Size > NeededTotalSize ) && ( ( BestFitCurrent == nullptr ) ||
                CurrentFreeBlock->Size < BestFitCurrent->Size ) ) {
                BestFitPrevious = PreviousFreeBlock;
                BestFitCurrent = CurrentFreeBlock;
                BestFitAdjustment = AdjustmentNeeded;
                BestFitTotalSize = NeededTotalSize;
            }

            PreviousFreeBlock = CurrentFreeBlock;
            CurrentFreeBlock = CurrentFreeBlock->Next;
        }

        if( BestFitCurrent == nullptr ) {
            return nullptr;
        }

        // Determine if our allocation will be possible.
        if( ( BestFitCurrent->Size - BestFitTotalSize ) <= sizeof( AllocationHeader ) ) {
            // Increase the allocation size instead of creating a new free block.
            BestFitTotalSize = BestFitCurrent->Size;

            if( BestFitPrevious != nullptr ) {
                BestFitPrevious->Next = BestFitCurrent->Next;
            } else {
                FreeBlocks = BestFitCurrent->Next;
            }
        } else {
            // Prevent the current new block from overwriting the best fit block.
            DJINN_ASSERT( BestFitTotalSize > sizeof( FreeBlock ) );

            FreeBlock* NewFreeBlock = reinterpret_cast< FreeBlock* >( BlockPointerArithmetic::Add( BestFitCurrent, BestFitTotalSize ) );
            NewFreeBlock->Size = BestFitCurrent->Size - BestFitTotalSize;
            NewFreeBlock->Next = BestFitCurrent->Next;

            #if DJINN_DEBUG
                NewFreeBlock->MagicNumber = AllocationMagicNumber;
            #endif

            if( BestFitPrevious != nullptr ) {
                BestFitPrevious->Next = NewFreeBlock;
            } else {
                FreeBlocks = NewFreeBlock;
            }
        }

        const Djinn::Handle AlignedAddress = reinterpret_cast< Djinn::Handle >( BestFitCurrent ) + BestFitAdjustment;

        AllocationHeader* const Header = reinterpret_cast< AllocationHeader* >( AlignedAddress - sizeof( AllocationHeader ) );
        Header->Size = BestFitTotalSize;
        Header->Adjustment = BestFitAdjustment;

        DJINN_ASSERT( BlockOperations::IsAligned( Header ) );

        #if DJINN_DEBUG
            Header->Identifier = NextIdentifier++;;
            Header->MagicNumber = AllocationMagicNumber;
        #endif

        Super->MemoryUsed += BestFitTotalSize;
        ++Super->AllocationCount;

        DJINN_ASSERT( BlockOperations::AlignForwardAdjustment( reinterpret_cast< void* >( AlignedAddress ), Alignment ) == 0 );

        #if DJINN_DEBUG
            ValidateBlockList();
        #endif

        return reinterpret_cast< void* >( AlignedAddress );
    }

    void Deallocate( void* const Block ) override {
        DJINN_ASSERT( Block != nullptr );

        AllocationHeader* const Header = reinterpret_cast< AllocationHeader* >( BlockPointerArithmetic::Subtract( Block, sizeof( AllocationHeader ) ) );

        #if DJINN_DEBUG
            const Djinn::USize Identifier = Header->Identifier;
            DJINN_ASSERT( Identifier < NextIdentifier );
            DJINN_ASSERT( Header->MagicNumber == AllocationMagicNumber );
        #endif

        Djinn::Handle BlockStart = reinterpret_cast< Djinn::Handle >( Block ) - Header->Adjustment;
        Djinn::USize BlockSize = Header->Size;
        Djinn::Handle BlockEnd = BlockStart + BlockSize;

        FreeBlock* PreviousFreeBlock = nullptr;
        FreeBlock* CurrentFreeBlock = FreeBlocks;

        while( CurrentFreeBlock != nullptr ) {
            if(reinterpret_cast< Djinn::Handle >( CurrentFreeBlock ) >= BlockEnd ) {
                break;
            }

            PreviousFreeBlock = CurrentFreeBlock;
            CurrentFreeBlock = CurrentFreeBlock->Next;
        }

        if( PreviousFreeBlock == nullptr ) {
            PreviousFreeBlock = reinterpret_cast< FreeBlock* >( BlockStart );
            PreviousFreeBlock->Size = BlockSize;
            PreviousFreeBlock->Next = FreeBlocks;

            #if DJINN_DEBUG
                PreviousFreeBlock->MagicNumber = AllocationMagicNumber;
            #endif

            FreeBlocks = PreviousFreeBlock;
        } else if( ( reinterpret_cast<Djinn::Handle >( PreviousFreeBlock ) + PreviousFreeBlock->Size ) == BlockStart ) {
            PreviousFreeBlock->Size += BlockSize;
        } else {
            FreeBlock* Temp  = reinterpret_cast< FreeBlock* >( BlockStart );
            Temp->Size = BlockSize;
            Temp->Next = PreviousFreeBlock->Next;

            #if DJINN_DEBUG
                Temp->MagicNumber = AllocationMagicNumber;
            #endif

            PreviousFreeBlock->Next = Temp;
            PreviousFreeBlock = Temp;
        }

        DJINN_ASSERT( PreviousFreeBlock != nullptr );

        if( ( reinterpret_cast< Djinn::Handle >( PreviousFreeBlock ) + PreviousFreeBlock->Size ) ==
            reinterpret_cast< Djinn::Handle >( PreviousFreeBlock->Next ) ) {
            PreviousFreeBlock->Size += PreviousFreeBlock->Next->Size;
            PreviousFreeBlock->Next = PreviousFreeBlock->Next->Next;
        }

        --Super->AllocationCount;
        Super->MemoryUsed -= BlockSize;

        #if DJINN_DEBUG
            ValidateBlockList();
        #endif
    }

#if DJINN_DEBUG
    void ValidateBlockList() {
        // Make sure that our blocks are correctly ordered in the list.
        FreeBlock* Block = FreeBlocks;

        while( Block != nullptr ) {
            DJINN_ASSERT( ( Block->Next == nullptr ) || ( Block < Block->Next ) );
            DJINN_ASSERT( Block->MagicNumber == AllocationMagicNumber );

            Block = Block->Next;
        }
    }
#endif

private:
    FreeListAllocator( const FreeListAllocator& Copy );
    FreeListAllocator& operator=( const FreeListAllocator& Rhs );

private:
    FreeBlock* FreeBlocks;

    #if DJINN_DEBUG
        Djinn::USize NextIdentifier;
    #endif
};

DJINN_END_NS2( Djinn, Memory )