// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.

#include <Core/Types.h>

#include <iostream>

#include <Core/MemoryDriver.h>
#include <Core/IAllocator.h>
#include <Core/FixedContiguousAllocator.h>
#include <Core/String.h>

#include <Configuration/ConfigFile.h>
#include <Configuration/ConfigFileManager.h>
#include <Host/IRegistry.h>


int main( Djinn::Int32 ArgCount, Djinn::Int8* Args[] ) {
    Djinn::Configuration::ConfigFile Config( "Test.conf" );



    //Djinn::Core::ConfigFileManager* Manager = Djinn::Core::ConfigFileManager::GetPointer();
    //Djinn::Core::ConfigFileManager::GetPointer()->RegisterDirectory( "Config" );

    const Djinn::USize PoolSize = ( 1024ULL * 1024 ) * 1024; // 1GB.
    Djinn::Memory::MemoryDriver* const MemDriver = Djinn::Memory::MemoryDriver::GetPointer();
    MemDriver->Startup( PoolSize );

    /*
    void* const InitialMemoryPool = malloc( PoolSize );
    Djinn::Memory::IAllocator* Allocator = new( InitialMemoryPool ) Djinn::Memory::FixedContiguousAllocator(
        ( PoolSize - sizeof( Djinn::Memory::FixedContiguousAllocator ) ), Djinn::Memory::BlockPointerArithmetic::Add( InitialMemoryPool,
        sizeof( Djinn::Memory::FixedContiguousAllocator ) ) );
    */

    //int* IntArray = Djinn::Memory::AllocateArray< Djinn::Int32 >( *Allocator, 5 );


    Djinn::Int32* const IntArray = MemDriver->AllocateArray< Djinn::Int32 >( 5 );

    IntArray[ 0 ] = 1;
    IntArray[ 1 ] = 2;
    IntArray[ 2 ] = 3;
    IntArray[ 3 ] = 4;
    IntArray[ 4 ] = 5;

    for( auto Index = 0; Index < 5; ++Index ) {
        std::cout << IntArray[ Index ] << std::endl;
    }

    MemDriver->DeallocateArray( IntArray );

    MemDriver->Shutdown();
    delete MemDriver;

    return 0;
}








































































// STD Map claims O( long n ) performance -> balanced tree.
// STD unordered map claims O( 1 ) support -> hash table.

//#define DJINN_MAP_INSERTIONS 10

//void Alert( const std::string& Msg ) {
//    std::cout << Msg << std::endl;
//}
//
//class PlatformIndependentTimer {
//private:
//    typedef std::chrono::high_resolution_clock Clock;
//    typedef std::chrono::duration< double, /*std::ratio< 1 >*/ std::milli > IncrementStep;
//
//public:
//    PlatformIndependentTimer() : Start( Clock::now() ) {
//    }
//
//    void Restart() {
//        Start = Clock::now();
//    }
//
//    std::string ElapsedAsString() {
//        return std::to_string( Elapsed() ) + "ms";
//    }
//
//    double Elapsed() const {
//        return std::chrono::duration_cast< IncrementStep >( Clock::now() - Start ).count();
//    }
//
//private:
//    std::chrono::time_point< Clock > Start;
//};

/*
struct HashableObject {
public:
    HashableObject() {
        std::random_device RandomDevice;
        std::mt19937_64 Generator( RandomDevice() );
        std::uniform_int_distribution< unsigned __int64 > Distribution( 0, 18446744073709551615 );

        Number = Distribution( Generator );
        Name = std::to_string( Number );
    }

public:
    std::string Name;
    unsigned __int64 Number; // 0 to 18,446,744,073,709,551,615
};
*/

//void __attribute__( (constructor) ) Premain() {
//    std::cout << "I've executed" << std::endl;
//}
//
//int main( int Argc, char* Argv[] ) {
//    //PlatformIndependentTimer Timer;
//
//    StringifiableObject Object;
//    Djinn::Nate::ToString( Object );

    //Djinn::Nate::HashMap< std::string, std::string > DjinnMap;
    //DjinnMap.Add( "Nat", "Han" );

    //DjinnMap.Add( 0, std::string( "Nathan" ) );
    //DjinnMap.Add( 1, "Mercy" );
    //DjinnMap.Add( 2, "Grant" );
    //DjinnMap.Add( 3, "Craig" );
    //DjinnMap.Add( 4, "Michael" );
    //DjinnMap.Add( 5, "Gemma" );
    //DjinnMap.Add( 6, "Nora" );

    //for( auto I = 0; I != DjinnMap.Count(); ++I ) {
        //std::cout << DjinnMap.ValueFromKey( I ) << std::endl;
    //}


//    Djinn::Nate::HashMap< unsigned int, HashableObject > Djinn_Map;
//
//    Alert( "[Djinn Map] Begin" );
//    Timer.Restart();
//
//    // Fill Djinn map.
//    for( auto Index = 0; Index < DJINN_MAP_INSERTIONS; ++Index ) {
//        Djinn_Map.Add( Index, HashableObject() );
//    }
//
//    for( auto Index = 0; Index < Djinn_Map.Count(); ++Index ) {
//            std::cout << Djinn_Map.ValueFromKey( Index ) << std::endl;
//    }
//
//    Alert( "[Djinn Map] End, Time Elapsed: " + Timer.ElapsedAsString() );
//
//    return 0;
//}