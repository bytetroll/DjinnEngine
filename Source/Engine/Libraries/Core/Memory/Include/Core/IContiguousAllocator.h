// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/MemoryToolbox.h>
#include <Core/IAllocator.h>
#include <Core/Assert.h>

// Base for allocators that allocate blocks in a linear, contiguous fashion.

DJINN_BEGIN_NS2( Djinn, Memory )

class IContiguousAllocator : public IAllocator {
public:
    IContiguousAllocator( const Djinn::USize Allotment, void* const BlockStart ) : IAllocator( Allotment ),
        BlockStart( BlockStart ), CurrentBlock( BlockStart ) {
    }

    //virtual ~IContiguousAllocator() {
    //}

    //  These allocators don't have any derived operations.  They need to use "Rewind" or "Clear" for operations.
    void Deallocate( void* const Block ) override final {
        DJINN_ASSERT( false, "Contigious allocators do not support direct deallocation" );
    }

    void* GetBlockStart() const {
        return BlockStart;
    }

    void* GetCurrentBlock() const {
        return CurrentBlock;
    }

    virtual void Rewind( void* const ToBlock ) = 0;
    virtual void Clear() = 0;

protected:
    void* BlockStart;
    void* CurrentBlock;
};

DJINN_END_NS2( Djinn, Memory )