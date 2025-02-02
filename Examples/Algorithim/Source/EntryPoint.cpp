// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.

// STL.
#include <iostream>

// Core
#include <Core/Types.h>
#include <Core/Algorithim.h>
#include <Core/Array.h>

using namespace Djinn;
using namespace Djinn::Core;

int main( Djinn::Int32 ArgCount, Djinn::Int8* Args[] ) {
    Array< UInt32 > Dicks = {
            1, 2, 3, 4, 5
    };

    // Print initial contents.
    for( auto Iter = Dicks.begin(); Iter != Dicks.end(); ++Iter ) {
        std::cout << (*Iter) << " ";
    }
    std::cout << std::endl;

    // Reverse and print contents.
    Reverse< UInt32 >( Dicks.begin(), Dicks.end() );
    for( auto Iter = Dicks.begin(); Iter != Dicks.end(); ++Iter ) {
        std::cout << (*Iter) << " ";
    }
    std::cout << std::endl;


    return 0;
}