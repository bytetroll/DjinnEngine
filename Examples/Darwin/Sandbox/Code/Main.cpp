//0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0
//>> Copyright (C) 2019 Djinn Engine Team.  All rights reserved.
//0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0

#include <Core/Types.h>
#include <Core/Console.h>

int main( int argc, char** argv ) {
#if DJINN_PLATFORM_DARWIN
    Djinn::Debug::Console::Print( "Hello, from Darwin!" );
#endif

    return 0;
}
