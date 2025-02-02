// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

// Use sparingly -- NCY -- 5.5.2018

#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/Assert.h>

#define DJINN_GCC_VISIBILITY

// Statics in a static lib only get initialized if called in code.  Thus, for static libs,
// template specialization won't work.  This forces initialization of our Singleton to a more
// traditional implementation of the Singleton -- checking each time a getter function is called.
#define DJINN_STATIC_LIB_PATCH 0

DJINN_BEGIN_NS2( Djinn, Core )

#ifdef DJINN_COMPILER_MSVC
    #pragma warning( disable: 4311 ) // Possible loss of data.
    #pragma warning( disable: 4312 ) // Truncation from type 1 to type 2 while converting.
    #pragma warning( disbale: 4661 ) // No suitable definition provided for explicit template instantiation.
#endif

#if defined( DJINN_GCC_VISIBILITY )
    #pragma GCC visibility push( default )
#endif

template< typename Type >
class Singleton {
public:
    Singleton() {
        //DJINN_ASSERT( Ptr != nullptr );

        #if defined( _MSC_VER ) && ( _MSC_VER < 1200 )
            const Djinn::Int32 Offset = (Djinn::Int32)( (Type*)1 - (Djinn::Int32)( Singleton< Type >* )( Type* )1 );
            Ptr = (Type*)( ( Djinn::Int32)this + Offset );
        #else
            Ptr = static_cast< Type* >( this );
        #endif
    }

    ~Singleton() {
        DJINN_ASSERT( Ptr );

        #ifdef DJINN_STATIC_LIB_PATCH
            delete Ptr;
        #endif

        Ptr = nullptr;
    }

    static Type& GetReference() {
        #if DJINN_STATIC_LIB_PATCH
                if( Ptr == nullptr ) {
                    Initialize();
                }
        #endif

        DJINN_ASSERT( Ptr );
        return *Ptr;
    }

//    static const Type& GetReference() const {
//        DJINN_ASSERT( Ptr );
//        return *Ptr;
//    }

    static Type* GetPointer() {
        #if DJINN_STATIC_LIB_PATCH
            if( Ptr == nullptr ) {
                Initialize();
            }
        #endif

        DJINN_ASSERT( Ptr );
        return Ptr;
    }

//    static const Type* const GetPointer() const {
//        DJINN_ASSERT( Ptr );
//        return Ptr;
//    }

private:

#ifdef DJINN_STATIC_LIB_PATCH
    static void Initialize() {
        #if defined( _MSC_VER ) && ( _MSC_VER < 1200 )
                const Djinn::Int32 Offset = (Djinn::Int32)( (Type*)1 - (Djinn::Int32)( Singleton< Type >* )( Type* )1 );
                    Ptr = (Type*)( ( Djinn::Int32)this + Offset );
        #else
                Ptr = new Type;
        #endif
    }
#endif

    Singleton( const Singleton< Type >& Copy ) {
    }

    Singleton& operator=( const Singleton< Type >& Rhs ) {
    }

protected:
    static Type* Ptr;
};

#if defined( DJINN_GCC_VISIBILITY )
    #pragma GCC visibility pop
#endif

DJINN_END_NS2( Djinn, Core )