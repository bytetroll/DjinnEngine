// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <algorithm>
#include <mutex>
#include <thread>

#include <Core/Common.h>
#include <Core/Size.h>
#include <Core/Hash.h>
#include <Core/Assert.h>

#include "Code/Hash/HashMapNode.h"

DJINN_BEGIN_NS2( Djinn, Nate )

#define DJINN_HASH_MAP_MSB_CLEAR 0x7FFFFFFF
#define DJINN_HASH_MAP_MSB_DELETE 0x80000000

// @Nate: Wrappers to control the movement of nodes and node pairs.
template< class Type >
Type& HashMapObjectMover( Type& Object ) {
    return std::move( Object );
}

template< class KeyType, class ValueType >
KeyValuePair< KeyType, ValueType >&& HashMapPairMover( const KeyValuePair< KeyType, ValueType >& Pair ) {
    return KeyValuePair< KeyType, ValueType >( std::move( Pair.Key ), std::move( Pair.Value ) );
};

template< class KeyType, class ValueType >
KeyValuePair< KeyType, ValueType >&& HashMapPairMover( const KeyType& Key, const ValueType& Value ) {
    return KeyValuePair< KeyType, ValueType >( std::move( Key ), std::move( Value ) );
};

// @Nate: This algorithm is currently experimental.  This hash map implements the round-robin hashing algorithm
//        which requires that a hash ID never be 0.
template< typename KeyType >
struct HashingAlgorithm_Djinn {
public:
    Djinn::USize operator()( const KeyType& Key ) const {
        return reinterpret_cast< Djinn::USize >( Djinn::Hash< KeyType >( Key ) );
    }
};

template< typename KeyType >
struct HashingAlgorithm_RoundRobin {
#include <functional>

public:
    Djinn::USize operator()( const KeyType& Key ) const {
        const std::hash< KeyType > Hasher{};
        Djinn::UInt32 Hash = static_cast< Djinn::UInt32 >( Hasher( Key ) );

        // The most significant bit indicates a deleted node so clear it.
        Hash &= DJINN_HASH_MAP_MSB_CLEAR;

        // Due to the round-robin algorithm, make sure the returned hash is never 0.
        Hash |= Hash == 0;

        return Hash;
    }
};

template< class KeyType, class ValueType, class HashingAlgorithm = HashingAlgorithm_RoundRobin< KeyType > >
class HashMap {
public:
    static const Djinn::UInt32 DEFAULT_SIZE = 256;
    static const Djinn::UInt32 LOAD_FACTOR_THRESHOLD = 90;
    static const Djinn::Int32 BAD_NODE_INDEX = -1;

public:
    HashMap() : Buffer( nullptr ), NodeCount( 0 ), MaxCapacity( DEFAULT_SIZE ), UseSeparateHashArray( false ) {
        AllocateBuffers();
    }

    HashMap( const Djinn::UInt32 Reserve ) : Buffer( nullptr ), NodeCount( 0 ), MaxCapacity( Reserve ),
        UseSeparateHashArray( false ) {
        AllocateBuffers();
    }

    HashMap( const bool UseSeparateHashArray ) : Buffer( nullptr ), NodeCount( 0 ), MaxCapacity( DEFAULT_SIZE ),
        UseSeparateHashArray( UseSeparateHashArray ) {
        AllocateBuffers();
    }

    HashMap( const Djinn::UInt32 Reserve, const bool UseSeparateHashArray ) : Buffer( nullptr ), NodeCount( 0 ),
        MaxCapacity( Reserve ), UseSeparateHashArray( UseSeparateHashArray ) {
        AllocateBuffers();
    }

    ~HashMap() {
        for( auto I = 0; I < MaxCapacity; ++I ) {
            if( LookupHash( I ) != 0 ) {
                Buffer[ I ].~HashMapNode();
            }
        }

        _aligned_free( Buffer );

        if( UseSeparateHashArray ) {
            delete[] Hashes;
        }
    }

    static DJINN_INLINE bool IsMarkedForDeletion( const Djinn::UInt32 Hash ) {
        return ( Hash >> 31 ) != 0;
    }

    void Add( const KeyType Key, const ValueType Value ) {
        //std::lock_guard< std::mutex > LockGuard( Mutex );
        if( ++NodeCount >= ResizeThreshold ) {
            Grow();
        }

        InsertNode( Hasher( Key ), std::move( Key ), std::move( Value ) );
    }

    bool Delete( const KeyType& Key ) {
        const Djinn::UInt32 Hash = Hasher( Key );
        const Djinn::Int32 Index = LookupIndex( Key );

        if( Index == BAD_NODE_INDEX ) {
            return false;
        }

        Buffer[ Index ].~HashMapNode();

        LookupHash( Index ) |= DJINN_HASH_MAP_MSB_DELETE;
        --NodeCount;

        return true;
    }

    ValueType* ValueFromKey( const KeyType& Key ) {
        const Djinn::UInt32 Hash = Hasher( Key );
        const Djinn::Int32 Index = LookupIndex( Key );

        return Index != BAD_NODE_INDEX ? &Buffer[ Index ].Pair.Value : nullptr;
    }

    HashMapNode< KeyType, ValueType >* FindPair( const KeyType& Key ) {
        const Djinn::UInt32 Hash = Hasher( Key );
        const Djinn::Int32 Index = LookupIndex( Key );

        return Index != BAD_NODE_INDEX ? &Buffer[ Index ].Pair : nullptr;
    };

    const HashMapNode< KeyType, ValueType >* const FindPair( const KeyType& Key ) const {
        return const_cast< HashMap< KeyType, ValueType >* >( this )->FindPair( Key );//LookupHash( Key );
    };

    Djinn::UInt32 Count() const {
        return NodeCount;
    }

    float AverageProbeCount() const {
        float ProbeTotal = 0.0f;

        for( auto I = 0; I < MaxCapacity; ++I ) {
            const Djinn::UInt32 Hash = LookupHash( I );
            if( ( Hash != 0 ) && ( !IsMarkedForDeletion( Hash ) ) ) {
                ProbeTotal += CalculateProbeDistance( Hash, I );
            }
        }

        return ( ProbeTotal / Count() ) + 1.0f;
    }

private:
    Djinn::UInt32& LookupHash( const Djinn::UInt32 Hash ) {
        if( UseSeparateHashArray ) {
            return Hashes[ Hash ];
        }

        return Buffer[ Hash ].Hash;
    }

    Djinn::UInt32 LookupHash( const Djinn::UInt32 Hash ) const {
        return const_cast< HashMap< KeyType, ValueType >* >( this )->LookupHash( Hash );
    }

    Djinn::Int32 LookupIndex( const KeyType& Key ) const {
        const Djinn::UInt32 Hash = Hasher( Key );
        Djinn::UInt32 Position = CalculateDesiredInsertionPoint( Hash );
        Djinn::UInt32 ProbeDistance = 0;

        for( ;; ) {
            if( LookupHash( Position ) == 0 ) {
                return BAD_NODE_INDEX;
            } else if( ProbeDistance > CalculateProbeDistance( LookupHash( Position ), Position ) ) {
                return BAD_NODE_INDEX;
            } else if( ( LookupHash( Position ) == Hash ) && ( Buffer[ Position ].Pair.Key == Key ) ) {
                return Position;
            }

            Position = ( Position + 1 ) & InsertionMask;
            ++ProbeDistance;
        }
    }

    void AllocateBuffers() {
        const Djinn::USize MallocSize = MaxCapacity * sizeof( HashMapNode< KeyType, ValueType > );
        const Djinn::USize MallocAlignment = __alignof( HashMapNode< KeyType, ValueType > );
        Buffer = reinterpret_cast< HashMapNode< KeyType, ValueType >* >( _aligned_malloc( MallocSize, MallocAlignment ) );

        if( UseSeparateHashArray ) {
            Hashes = new Djinn::UInt32[ MaxCapacity ];
        }

        // Flag all newly allocated nodes as free.
        for( auto Index = 0; Index < MaxCapacity; ++Index ) {
            LookupHash( Index ) = 0;
        }

        ResizeThreshold = ( MaxCapacity * LOAD_FACTOR_THRESHOLD ) / 100; // * 0.01;
        InsertionMask = MaxCapacity - 1;
    }

    void ConstructNode( const Djinn::UInt32 Index, const Djinn::UInt32 Hash, const KeyType&& Key, const ValueType&& Value ) {
        new( &Buffer[ Index ] ) HashMapNode< KeyType, ValueType >( /*HashMapPairMover( Key, Value )*/ std::move( Key ), std::move( Value ) );
        LookupHash( Index ) = Hash;
    }

    void InsertNode( const Djinn::UInt32 Hash, const KeyType&& Key, const ValueType&& Value ) {
        Djinn::UInt32 PotentialInsertionPosition = CalculateDesiredInsertionPoint( Hash );
        Djinn::UInt32 ProbeDistance = 0;

        for( ;; ) {
            if( LookupHash( PotentialInsertionPosition ) == 0 ) {
                ConstructNode( PotentialInsertionPosition, Hash, std::move( Key ), std::move( Value ) ); //HashMapObjectMover( Key ), HashMapObjectMover( Value ) );
                return;
            }

            // If the existing node has been probed less than this node, swap the places and keep going until
            // we find another open slot.
            Djinn::UInt32 ExistingNodeProbeDistance = CalculateProbeDistance( LookupHash( PotentialInsertionPosition ), PotentialInsertionPosition );
            if( ExistingNodeProbeDistance < ProbeDistance ) {
                if( IsMarkedForDeletion( LookupHash( PotentialInsertionPosition ) ) ) {
                    ConstructNode( PotentialInsertionPosition, Hash, std::move( Key ), std::move( Value ) );//HashMapObjectMover( Key ), HashMapObjectMover( Value ) );
                    return;
                }

                // Wrap these like the mover's.  std::swap may need to be swapped out later on for performance
                // reasons.
                std::swap( (Djinn::UInt32&)Hash, LookupHash( PotentialInsertionPosition ) );
                std::swap( (KeyType&)Key, Buffer[ PotentialInsertionPosition ].Pair.Key );
                std::swap( (ValueType&)Value, Buffer[ PotentialInsertionPosition ].Pair.Value );

                ProbeDistance = ExistingNodeProbeDistance;
            }

            PotentialInsertionPosition = ( PotentialInsertionPosition + 1 ) & InsertionMask;
            ++ProbeDistance;
        }
    }

    void Grow() {
        const Djinn::UInt32 OldCapacity = MaxCapacity;
        HashMapNode< KeyType, ValueType >* OldNodes = Buffer;

        Djinn::UInt32* OldHashes = nullptr;
        if( UseSeparateHashArray ) {
            OldHashes = Hashes;
        }

        MaxCapacity *= 2;
        AllocateBuffers();

        // Copy over old nodes to the newly resized buffers.
        for( auto I = 0; I < OldCapacity; ++I ){
            const auto& Node = OldNodes[ I ];

            Djinn::UInt32 Hash = 0;
            if( UseSeparateHashArray ) {
                Always( OldHashes != nullptr );
                Hash = OldHashes[ I ]; //static_cast< Djinn::UInt32 >( &OldHashes[ I ] );
            } else {
                Hash = Node.Hash;
            }

            if( ( Hash != 0 ) && ( !IsMarkedForDeletion( Hash ) ) ) {
                // @TODO: These MIGHT need to be moved with std::move
                InsertNode( Hash, std::move( Node.Pair.Key ), std::move( Node.Pair.Value ) );
                Node.~HashMapNode();
            }
        }

        _aligned_free( OldNodes );

        if( UseSeparateHashArray ) {
            Always( OldHashes != nullptr );
            delete[] OldHashes;
        }
    }

    DJINN_INLINE Djinn::UInt32 CalculateDesiredInsertionPoint( const Djinn::UInt32 Hash ) const {
        return Hash & InsertionMask;
    }

    DJINN_INLINE Djinn::UInt32 CalculateProbeDistance( const Djinn::UInt32 Hash, const Djinn::UInt32 SlotIndex ) const {
        return ( ( SlotIndex + MaxCapacity ) - CalculateDesiredInsertionPoint( Hash ) ) & InsertionMask;
    }

private:
    HashingAlgorithm Hasher;

    // Since round-robin hashing likes to move stuff around, ensure that our pointers never overlap.
    // __restrict is a compiler extension and is supported by most major compilers. If building on an unsupported
    // compiler, an alternative to __restrict will have to be created.
    HashMapNode< KeyType, ValueType >* __restrict Buffer;
    Djinn::UInt32* __restrict Hashes;

    Djinn::UInt32 NodeCount;

    Djinn::UInt32 MaxCapacity;
    Djinn::UInt32 ResizeThreshold;

    Djinn::UInt32 InsertionMask;

    // If true, a separate array will be used for storing hash's.  This will also store each hash individually
    // in the individual nodes.
    bool UseSeparateHashArray;

    std::mutex Mutex;
};

DJINN_END_NS2( Djinn, Nate )