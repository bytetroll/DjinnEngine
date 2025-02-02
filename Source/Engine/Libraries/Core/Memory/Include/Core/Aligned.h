// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <Core/Common.h>
#include <Core/Size.h>

// These are for standard heap allocations using the STL allocators.  These are not for
// our custom memory allocators... those have their own routines. - NCY.
// https://github.com/NickStrupat/AlignedMalloc

// @TODO: Memory should be moved to Djinn::Memory namespace? - NCY
DJINN_BEGIN_NS2( Djinn, Core )

void* Djinn_Memory_AlignedMalloc( const Djinn::USize& Size, const Djinn::USize& Alignment );
void Djinn_Memory_AlignedFree( void* const Block );

DJINN_END_NS2( Djinn, Core )