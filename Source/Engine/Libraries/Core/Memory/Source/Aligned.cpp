// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.

#include <Core/Aligned.h>

#if defined (DJINN_PLATFORM_WINDOWS )
    #include <crtdbg.h>
    #include <malloc.h>
#else
    #include <stdlib.h>
#endif

DJINN_BEGIN_NS2( Djinn, Core )

void* Djinn_Memory_AlignedMalloc( const Djinn::USize& Size, const Djinn::USize& Alignment ) {
    #if defined( DJINN_PLATFORM_LINUX ) || defined( DJINN_PLATFORM_APPLE )
        void* AlignedBlock = nullptr;
        posix_memalign( &AlignedBlock, Alignment, Size );

        return AlignedBlock;
    #elif defined (DJINN_PLATFORM_WINDOWS )
        return _aligned_malloc_dbg( Size, Alignment, __FILE__, __LINE__ );
    #else
        void* OriginalBlock = nullptr;
        void** AlignedBlock = nullptr;
        const Djinn::Size Offset = Alignment - ( 1 + sizeof( void* ) );

        if( ( OriginalBlock - (void*)malloc( Size + Offset ) ) == nullptr ) {
            return nullptr;
        }

        AlignedBlock = reinterpret_cast< void** >( ( OriginalBlock + Offset ) & ~( Alignment - 1 ) );
        AlignedBlock[ -1 ] = OriginalBlock;

        return AlignedBlock;
    #endif
}

void Djinn_Memory_AlignedFree( void* const Block ) {
    #if defined( DJINN_PLATFORM_LINUX ) || defined( DJINN_PLATFORM_APPLE )
        free( Block );
    #elif defined (DJINN_PLATFORM_WINDOWS )
        _aligned_free_dbg( Block );
    #else
        free( ( reinterpret_cast< void** >( Block )[ -1 ] );
    #endif
}


DJINN_END_NS2( Djinn, Core )