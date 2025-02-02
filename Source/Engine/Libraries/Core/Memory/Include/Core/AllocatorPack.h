// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <Core/Types.h>
#include <Core/Common.h>

#include <Core/IAllocator.h>
#include <Core/IAllocatorProxy.h>

// Allocator pack represents a created, bound allocator and proxy pair.
// The memory driver returns allocator packs for all allocator creation.
// The allocator pack ensures that every new allocator created has a bound,
// valid proxy associated with the allocator.  The allocator pack can used
// in anything that requires an allocator through the pointer dereference,
// which guarantees that all operations are done through the proxy. -- NCY

DJINN_BEGIN_NS2( Djinn, Memory )

class AllocatorPack {
public:
    explicit AllocatorPack( IAllocator* const Allocator, IAllocatorProxy* const AllocatorProxy ) :
            Allocator( Allocator ), AllocatorProxy( AllocatorProxy ) {
    }

    // @TODO: Need to handle copy constructors n' shit.

    IAllocator& operator*() {
        return *AllocatorProxy;
    }

    IAllocator& operator*() const {
        return *AllocatorProxy;
    }

private:
    IAllocator* Allocator;
    IAllocatorProxy* AllocatorProxy;
};

DJINN_END_NS2( Djinn, Memory )