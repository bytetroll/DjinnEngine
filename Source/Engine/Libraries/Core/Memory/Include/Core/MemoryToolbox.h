// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <Core/Size.h>
#include <Core/Common.h>

DJINN_BEGIN_NS2( Djinn, Memory )

//#ifdef __cplusplus
//extern "C" {
//#endif

// Block pointer arithmetic.
class BlockPointerArithmetic {
public:
    DJINN_INLINE static void* Add( void* const Pointer, const Djinn::USize Amount ) {
        return reinterpret_cast< void* >( reinterpret_cast< Djinn::Handle >( Pointer ) + Amount );
    }

    DJINN_INLINE static const void* Add( const void* const Pointer, const Djinn::USize Amount ) {
        return reinterpret_cast< const void* >( reinterpret_cast< Djinn::Handle >( Pointer ) + Amount );
    }

    DJINN_INLINE static void* Subtract( void* const Pointer, const Djinn::USize Amount ) {
        return reinterpret_cast< void* >( reinterpret_cast< Djinn::Handle >( Pointer ) - Amount );
    }

    DJINN_INLINE static const void* Subtract( const void* const Pointer, const Djinn::USize Amount ) {
        return reinterpret_cast< const void* >( reinterpret_cast< Djinn::Handle >( Pointer ) - Amount );
    }
};

// Block Alignment.
class BlockOperations {
public:
    DJINN_INLINE static void* AlignForward( void* const Block, Djinn::UInt8 Alignment ) {
        const Djinn::Handle BlockAddress = reinterpret_cast< Djinn::Handle >( Block );
        const Djinn::Handle ComputedAlignment = static_cast< Djinn::Handle >( Alignment - 1 );
        const Djinn::Handle Mask = static_cast< Djinn::Handle >( ~( Alignment - 1 ));

        return reinterpret_cast< void* >(( BlockAddress + ComputedAlignment ) & Mask );
    }

    DJINN_INLINE static const void* AlignForward( const void* const Block, Djinn::UInt8 Alignment ) {
        const Djinn::Handle BlockAddress = reinterpret_cast< Djinn::Handle >( Block );
        const Djinn::Handle ComputedAlignment = static_cast< Djinn::Handle >( Alignment - 1 );
        const Djinn::Handle Mask = static_cast< Djinn::Handle >( ~( Alignment - 1 ));

        return reinterpret_cast< void* >(( BlockAddress + ComputedAlignment ) & Mask );
    }

    DJINN_INLINE static Djinn::UInt8 AlignForwardAdjustment( const void* const Block, Djinn::UInt8 Alignment ) {
        const Djinn::Handle BlockAddress = reinterpret_cast< Djinn::Handle >( Block );
        const Djinn::Handle Mask = static_cast< Djinn::Handle >( Alignment - 1 );

        const Djinn::UInt8 Adjustment = Alignment - ( BlockAddress & Mask );
        if( Adjustment == Alignment ) {
            return 0; // We're already correctly aligned.
        }

        return Adjustment;
    }

    template< class Type >
    DJINN_INLINE static Djinn::UInt8 AlignForwardAdjustmentWithHeader( const void* const Block, Djinn::UInt8 Alignment ) {
        if( __alignof( Type ) > Alignment ) {
            Alignment = __alignof( Type );
        }

        return sizeof( Type ) +
               AlignForwardAdjustment( BlockPointerArithmetic::Add( Block, sizeof( Type )), Alignment );
    }

    DJINN_INLINE static void* AlignBackward( void* const Block, Djinn::UInt8 Alignment ) {
        const Djinn::Handle BlockAddress = reinterpret_cast< Djinn::Handle >( Block );
        const Djinn::Handle Mask = static_cast< Djinn::Handle >( ~( Alignment - 1 ));

        return reinterpret_cast< void* >( BlockAddress & Mask );
    }

    DJINN_INLINE static const void* AlignBackward( const void* const Block, Djinn::UInt8 Alignment ) {
        const Djinn::Handle BlockAddress = reinterpret_cast< Djinn::Handle >( Block );
        const Djinn::Handle Mask = static_cast< Djinn::Handle >( ~( Alignment - 1 ));

        return reinterpret_cast< void* >( BlockAddress & Mask );
    }

    DJINN_INLINE static Djinn::UInt8 AlignBackwardAdjustment( const void* const Block, Djinn::UInt8 Alignment ) {
        const Djinn::Handle BlockAddress = reinterpret_cast< Djinn::Handle >( Block );
        const Djinn::Handle Mask = static_cast< Djinn::Handle >( Alignment - 1 );

        const Djinn::UInt8 Adjustment = Alignment - ( BlockAddress & Mask );
        if( Adjustment == Alignment ) {
            return 0; // We're already correctly aligned.
        }

        return Adjustment;
    }

    // Block Alignment checks.
    DJINN_INLINE static bool IsAligned( const void* const Block, Djinn::UInt8 Alignment ) {
        return AlignForwardAdjustment( Block, Alignment ) == 0;
    }

    template< class Type >
    DJINN_INLINE static bool IsAligned( const Type* const Block ) {
        return AlignForwardAdjustment( Block, __alignof( Type )) == 0;
    }
};

//#ifdef __cplusplus
//}
//#endif

DJINN_END_NS2( Djinn, Memory )