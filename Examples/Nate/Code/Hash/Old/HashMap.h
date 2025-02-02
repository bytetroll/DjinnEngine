// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

// Use Djinn::Hasher -> Engine:Libraries:Core:Hash:Include:Core.

#include <Core/Common.h>
#include <Core/Hash.h>
#include <mutex>
#include <condition_variable>
#include <Core/Assert.h>

#include "HashMapBucket.h"
#include "HashMapIterator.h"

#define DJINN_HASH_TABLE_STARTING_SIZE 25
#define DJINN_HASH_TABLE_EXPAND_AMOUNT 50

// @TODO: Change these namespaces after development to Djinn::Core
DJINN_BEGIN_NS2( Djinn, Nate )

template< typename KeyType >
struct HashingAlgorithm_Djinn {
public:
    Djinn::USize operator()( const KeyType& Key ) const {
        return reinterpret_cast< Djinn::USize >( Djinn::Hash< KeyType >( Key ) );
    }
};

template< class KeyType, class ValueType, class HashingAlgorithm = HashingAlgorithm_Djinn< KeyType > >
class HashMap {
public:
     using Iterator = HashMapIterator< KeyType, ValueType >;
    //typedef HashMapConstantIterator< KeyType, ValueType > ConstantIterator;

public:
    HashMap( bool ZeroOut = true ) {
        BackingArray = new HashMapBucket< KeyType, ValueType > *[ DJINN_HASH_TABLE_STARTING_SIZE ]();
        BackingArraySize = DJINN_HASH_TABLE_STARTING_SIZE;
        BackingArraySlotsUsed = 0;

        this->ZeroOut = ZeroOut;
        if( ZeroOut ) {
            for( auto Index = 0; Index != BackingArraySize; Index++ ) {
                //*BackingArray[ Index ] = nullptr;
            }
        }
    }

    ~HashMap() {
        for( auto Index = 0; Index < BackingArraySize; ++Index ) {
            HashMapBucket< KeyType, ValueType >* Bucket = BackingArray[ Index ];

            // Iterate and clean from back to front.
            while( Bucket != nullptr ) {
                HashMapBucket< KeyType, ValueType >* PreviousBucket = Bucket;
                Bucket = Bucket->Next;

                delete PreviousBucket;
            }

            //BackingArray[ Index ] = nullptr;
        }

        delete[] BackingArray;
    }

    // Add a new key/value pair to the hash map.
    void Add( const KeyType& Key, const ValueType& Value ) {
        // @TODO: Resize backing array here.
        if( BackingArraySlotsUsed == BackingArraySize ) {
            ResizeBackingArray( DJINN_HASH_TABLE_EXPAND_AMOUNT );
        }

        const Djinn::UInt64 ComputedHash = Hasher( Key );

        HashMapBucket< KeyType, ValueType >* Bucket = BackingArray[ ComputedHash ];
        HashMapBucket< KeyType, ValueType >* PreviousBucket = nullptr;

        while( ( Bucket != nullptr ) && ( Bucket->Key != Key ) ) {
            PreviousBucket = Bucket;
            Bucket = Bucket->Next;
        }

        // We do not have a collision.
        if( Bucket == nullptr ) {
            Bucket = new HashMapBucket< KeyType, ValueType >( Key, Value );

            if( PreviousBucket == nullptr ) {
                // Insert as the first bucket in the map.
                BackingArray[ ComputedHash ] = Bucket;
            } else {
                PreviousBucket->Value = Value;
            }

            ++BackingArraySlotsUsed;
        } else {
            // Entry has already been stored -- just update value.
            Bucket->Value = Value;
        }
    }

    HashMapBucket< KeyType, ValueType >* Lookup( const Djinn::USize Index ) const {
        return BackingArray[ Index ] != nullptr ? BackingArray[ Index ] : nullptr;
    }

    Djinn::UInt32 Used() const {
        return BackingArraySlotsUsed;
    }

    Djinn::UInt32 Capacity() const {
        return BackingArraySize;
    }

    // Iterator support.
    Iterator Start() {
        /*
        return Iterator {
                BackingArray[ 0 ]
        };
         */

        return Iterator {
                BackingArray[ 0 ]
        };

        /*
        return {
                BackingArray[ 0 ]
        };
         */
    }

    Iterator End() {
        return Iterator {
                BackingArray[ BackingArraySize - 1 ]
        };


        /*
        return {
                BackingArray[ BackingArraySize ]
        };
         */
    }

    // Operators.
    HashMapBucket< KeyType, ValueType >& operator[]( Djinn::USize Index ) {
        //return reinterpret_cast< HashMapBucket< KeyType, ValueType >& >( Lookup( Index ) );
        return *Lookup( Index );
    }

    HashMapBucket< KeyType, ValueType >& operator[]( Djinn::USize Index ) const {
        //return reinterpret_cast< HashMapBucket< KeyType, ValueType >& >( Lookup( Index ) );
        return *Lookup( Index );
    }

    /*
    ConstantIterator Start() const {
        return ConstantIterator {
                BackingArray[ 0 ]
        };
    }

    ConstantIterator End() const {
        return ConstantIterator {
                BackingArray[ BackingArraySize ]
        };
    }
    */

private:
    void ResizeBackingArray( const Djinn::UInt32 By ) {
        std::lock_guard< std::mutex > LockGuard( Mutex );

        HashMapBucket< KeyType, ValueType > **PreviousBackingArray = nullptr;
        try {
            PreviousBackingArray = new HashMapBucket< KeyType, ValueType > *[ BackingArraySize ]();
        } catch( const char* const Message ) {
            std::cout << Message << std::endl;
        }

        // Copy old elements for previous range.
        // @TODO: Some serious optimization.
        for( auto Index = 0; Index != BackingArraySize; ++Index ) {
            PreviousBackingArray[ Index ] = BackingArray[ Index ];
        }

        // We want to keep the object pointers in tact -- just delete the backing array.
        delete[] BackingArray;

        const Djinn::UInt32 NewSize = BackingArraySize + By;

        // Reallocate our backing array.
        BackingArray = new HashMapBucket< KeyType, ValueType > *[ NewSize ]();

        // Copy back the old elements.
        // @TODO: May need some major optimization like above.
        for( auto Index = 0; Index != BackingArraySize; ++Index ) {
            BackingArray[ Index ] = PreviousBackingArray[ Index ];
        }

        // If we're set to initially zero out, zero out the remainder elements so we have a nice
        // clean slate.
        if( ZeroOut ) {
            for( auto Index = By; Index != NewSize; ++Index ) {
                //*BackingArray[ Index ] = nullptr;
            }
        }

        BackingArraySize = NewSize;
    }

private:
    HashMapBucket< KeyType, ValueType >** BackingArray;
    Djinn::UInt32 BackingArraySize;
    Djinn::UInt32 BackingArraySlotsUsed;

    bool ZeroOut;

    HashingAlgorithm Hasher;

    // Threading.
    std::mutex Mutex;
    std::condition_variable Condition;
};

DJINN_END_NS2( Djinn, Nate )
