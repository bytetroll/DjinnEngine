// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

// Memory driver overview.  Memory driver is primarily responsible for allocating, managing, and freeing
// our initial memory pool.  Pool is split into two sections, the heap, and the stack.  The heap is
// is managed by a free list allocator, which implements the free list heap algorithm.  The stack
// implementation is still up for discussion.  All sub-allocators/allocations are done through
// the heap allocator (proxy).

// @TODO: Need to wrap proxy and allocator's in class with pointer only operator
//        access so that when an allocator is created, it's proxy is created and
//        registered.  This ensures that we don't get a stray allocator without
//        a proxy.

#include <Core/Types.h>
#include <Core/Common.h>

#include <Core/IAllocator.h>
#include <Core/IAllocatorProxy.h>
#include <Core/IAllocatorFactory.h>

#include <Core/AllocatorPack.h>

#include <Core/FreeListAllocator.h>
#include <Core/Singleton.h>

DJINN_BEGIN_NS2( Djinn, Memory )

#define DJINN_DEFAULT_POOL_SIZE ( 1024 * 1024 ) * 1024 // 1GB

class MemoryDriver : public Djinn::Core::Singleton< MemoryDriver > {
public:
    MemoryDriver() {
    }

    void Startup( const Djinn::USize PoolSize );
    void Shutdown();

    // @TODO: Below functions were quickly moved.  Anything that takes IAllocator is considered global.
    //        A second set of matching functions should be added that forgoes the IAllocator argument
    //        automatically using the main allocator below.
    template< class AllocType, class... Args >
    AllocType* AllocateNew( IAllocator& Allocator, const Args&&... AllocArgs ) {
        return new( Allocator.Allocate( sizeof( AllocType ), __alignof( AllocType ) ) ) AllocType( std::forward< Args >( AllocArgs )... );
    }

    template< class AllocType, class... Args >
    AllocType* AllocateNew( const Args&&... AllocArgs ) {
        return AllocateNew< AllocType, Args... >( *HeapAllocatorProxy, AllocArgs... );
    };

    // Will call placement new on all allocations in array.
    template< class AllocType >
    AllocType* AllocateArray( IAllocator& Allocator, const Djinn::USize Length ) {
        DJINN_ASSERT( Length != 0 );

        Djinn::UInt8 HeaderSize = sizeof( Djinn::USize ) / sizeof( AllocType );
        if( ( sizeof( Djinn::USize ) % sizeof( AllocType ) ) > 0 ) {
            ++HeaderSize;
        }

        // Extra space is allocated at the start of the block to store the length of the array.
        const Djinn::USize NewBlockSize = ( Length + HeaderSize ) * sizeof( AllocType );
        AllocType* const NewBlock = reinterpret_cast< AllocType* >( Allocator.Allocate( NewBlockSize, __alignof( AllocType ) ) ) + HeaderSize;

        *( reinterpret_cast< Djinn::USize* >( NewBlock ) - 1 ) = Length;

        // Construct each sub-block in the array for convenience.
        // @Nathan: Cast in loop done quickly to get Linux building under Clang.  This needs to be fixed
        //          as it adds performance overhead.
        for( auto SubBlockIndex = 0; SubBlockIndex != static_cast<Djinn::Int32>(Length); ++SubBlockIndex ) {
            new( &NewBlock[ SubBlockIndex ] ) AllocType;
        }

        return NewBlock;
    }

    template< class AllocType >
    AllocType* AllocateArray( const Djinn::USize Length ) {
        return AllocateArray< AllocType >( *HeapAllocatorProxy, Length );
    }


    template< class AllocType >
    AllocType* AllocateArrayNoConstruct( IAllocator& Allocator, const Djinn::USize Length ) {
        DJINN_ASSERT( Length != 0 );

        Djinn::UInt8 HeaderSize = sizeof( Djinn::USize ) / sizeof( AllocType );
        if( ( sizeof( Djinn::USize ) / sizeof( AllocType ) ) > 0 ) {
            ++HeaderSize;
        }

        // Extra space is allocated at the start of the block to store the length of the array.
        const Djinn::USize NewBlockSize = ( Length + HeaderSize ) * sizeof( AllocType );
        AllocType* const NewBlock = reinterpret_cast< AllocType* >( Allocator.Allocate( NewBlockSize, __alignof( AllocType ) ) ) + HeaderSize;
        *( (reinterpret_cast< Djinn::USize* >( NewBlock ) - 1  ) ) = Length;

        return NewBlock;
    }

    template< class AllocType >
    AllocType* AllocateArrayNoConstruct( const Djinn::USize Length ) {
        return AllocateArrayNoConstruct< AllocType >( *HeapAllocatorProxy, Length );
    }

    template< class AllocType >
    void DeallocateDelete( IAllocator& Allocator, AllocType* const Block ) {
        DJINN_ASSERT( Block != nullptr );

        Block->~AllocType();
        Allocator.Deallocate( Block );
    }

    template< class AllocType >
    void DeallcoateDelete( AllocType* const Block ) {
        DeallocateDelete< AllocType >( *HeapAllocatorProxy, Block );
    }

    template< class AllocType >
    void DeallocateArray( IAllocator& Allocator, AllocType* const Block ) {
        DJINN_ASSERT( Block != nullptr );

        const Djinn::USize Length = *( ( reinterpret_cast< Djinn::USize* >( Block ) - 1 ) );
        for( auto Index = 0; Index < Length; ++Index ) { // These should be in the reverse order.
            Block[ Index ].~AllocType();
        }

        Djinn::UInt8 HeaderSize = sizeof( Djinn::USize ) / sizeof( AllocType );
        if( sizeof( Djinn::USize ) % sizeof( AllocType ) ) {
            ++HeaderSize;
        }

        Allocator.Deallocate( Block - HeaderSize );
    }

    template< class AllocType >
    void DeallocateArray( AllocType* const Block ) {
        DeallocateArray< AllocType >( *HeapAllocatorProxy, Block );
    }

    template< class AllocType >
    void DeallocateArrayNoDestruct( IAllocator& Allocator, AllocType* const Block ) {
        DJINN_ASSERT( Block != nullptr );

        // Determine block header size.
        Djinn::UInt8 HeaderSize = sizeof( Djinn::USize ) / sizeof( AllocType );
        if( ( sizeof( Djinn::USize ) % sizeof( AllocType ) ) > 0 ) {
            ++HeaderSize;
        }

        // We do not want to destroy the allocation headers.  This allows the profiler
        // to do what we need it to do -- NCY.
        Allocator.Deallocate( Block - HeaderSize );
    }

    template< class AllocType >
    void DeallocateArrayNoDestruct( AllocType* const Block ) {
        DeallocateArrayNoDestruct< AllocType >( *HeapAllocatorProxy, Block );
    }

    // @TODO: Allocator factory stuff/creation should be thrown here.
    //        It makes sense to let the memory driver create the allocators. -- NCY
    template< class AllocatorType >
    AllocatorType* CreateAllocator( const Djinn::USize Allotment ) {
        // We need to use the main allocator proxy's allocator. -- NCY.
        return IAllocatorFactory< AllocatorType >::Create( Allotment, *HeapAllocatorProxy );
    }

    // TESTING -- NCY.
    template< class AllocatorType >
    AllocatorPack* CreatePackedAllocator( const Djinn::USize Allotment ) {
        IAllocator* const Allocator = CreateAllocator< AllocatorType >( Allotment );
        IAllocatorProxy* const AllocatorProxy = CreateAllocatorProxy( Allocator );

        return new AllocatorPack( Allocator, AllocatorProxy );
    }


    template< class AllocatorType >
    void DestroyAllocator( AllocatorType& Allocator ) {
        // See note above. -- NCY.
        IAllocatorFactory< AllocatorType >::Destroy( *HeapAllocatorProxy, Allocator );
    }

    // The way these two functions operation MIGHT need to be changed when new/delete are overloaded globally.
    // It might also be useful to assume that all created allocators will be bound to an allocator proxy.  In which
    // case, the two can be wrapped in a class, constructed together, and returned as an object... maybe with
    // the pointer access operator allowing access to the proxy only -- i.e., all allocations happen through the
    // proxy. -- NCY.
    IAllocatorProxy* CreateAllocatorProxy( IAllocator* const Allocator ) {
        return new IAllocatorProxy( *Allocator );
    }

    void DestroyAllocatorProxy( IAllocatorProxy* const AllocatorProxy ) {
        if( AllocatorProxy != nullptr ) {
            delete AllocatorProxy;
        }
    }

    Djinn::USize GetAllocatedPoolSize() const {
        return AllocatedPoolSize;
    };

    IAllocator* GetHeapAllocator() const {
        return HeapAllocator;
    }

    IAllocatorProxy* GetHeapAllocatorProxy() const {
        return HeapAllocatorProxy;
    }

private:
    // Singleton implementation.
    MemoryDriver( const MemoryDriver& Copy );
    MemoryDriver& operator=( const MemoryDriver& Rhs );

private:
    Djinn::USize AllocatedPoolSize;
    void* AllocatedPool;

    // Main allocator responsible for managing our pool of memory.
    IAllocator* HeapAllocator;
    IAllocatorProxy* HeapAllocatorProxy;
};

DJINN_END_NS2( Djinn, Memory )