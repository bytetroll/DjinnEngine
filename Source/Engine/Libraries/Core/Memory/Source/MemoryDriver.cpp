// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.

#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/Assert.h>
#include <Core/MemoryDriver.h>

#include <Core/Singleton.h>

#include <Core/FixedContiguousAllocator.h>

template<> Djinn::Memory::MemoryDriver* Djinn::Core::Singleton< Djinn::Memory::MemoryDriver >::Ptr = nullptr;

DJINN_BEGIN_NS2( Djinn, Memory )

//void* MemoryDriver::AllocatedPool = nullptr;

void MemoryDriver::Startup( const Djinn::USize PoolSize ) {
    DJINN_ASSERT( PoolSize != 0 );
    AllocatedPoolSize = PoolSize;
    AllocatedPool = std::malloc( PoolSize );
    DJINN_ASSERT( AllocatedPool != nullptr, ("Failed to allocated pool with size " + std::to_string( PoolSize )).c_str() );

    // @TODO: This needs to be changed later.  Right now default to a fixed contiguous allocator. -- NCY.
    HeapAllocator = new( AllocatedPool ) FreeListAllocator(
            ( PoolSize - sizeof( FreeListAllocator ) ), BlockPointerArithmetic::Add( AllocatedPool,
            sizeof( FreeListAllocator ) ) );
    HeapAllocatorProxy = CreateAllocatorProxy( HeapAllocator );
}

void MemoryDriver::Shutdown() {
    if( AllocatedPool != nullptr ) {
        std::free( AllocatedPool );
        AllocatedPool = nullptr;
    }
}

DJINN_END_NS2( Djinn, Memory )