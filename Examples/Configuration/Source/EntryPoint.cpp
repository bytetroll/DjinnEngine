// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.

// STL.
#include <iostream>

// Core
#include <Core/Types.h>
#include <Core/Algorithim.h>
#include <Core/Array.h>

#include <Configuration/ConfigFile.h>
#include <Base/Application.h>
#include <Configuration/ConfigFileManager.h>

#include <Platform/FileSystem.h>

#include <Core/RegExpr.h>
#include <Core/Console.h>

DJINN_NS2(Core);

void _main( Base::Application &app ) {
    // Load configuration plugin.
    Always( app.GetRegistry()->LoadPlugin("Plugins.Configuration" ) );

    // One off config files.
    Djinn::Configuration::ConfigFile Config( "Test.conf" );

    const bool Daddy = Config.ReadBool( "TouchMe", "Daddy", false );
    const bool Migi = Config.ReadBool( "TouchMe", "Migi", false );
    const bool Stranger = Config.ReadBool( "TouchMe", "Stranger", true );

    const Djinn::Int32 Have = Config.ReadInt( "Virgins", "Have", -1 );
    const Djinn::Int32 Need = Config.ReadInt( "Virgins", "Need", -1 );

    const float HaveF = Config.ReadFloat( "Virgins", "Have", -1.0f );
    const float NeedF = Config.ReadFloat( "Virgins", "Need", -1.0f );

    const double HaveD = Config.ReadDouble( "Virgins", "Have", -1.0f );
    const double NeedD = Config.ReadDouble( "Virgins", "Need", -1.0f );

    const Djinn::Core::String Penis = Config.ReadString( "Parts", "Penis", "Boobs" );
    const Djinn::Core::String Vagina = Config.ReadString( "Parts", "Vagina", "Boobs" );
    const Djinn::Core::String Boobs = Config.ReadString( "Boobs", "Boobs", "Penis" );

    // Fetch the config file manager plugin hook.
    Configuration::ConfigFileManager* Manager = nullptr;
    Always( app.GetRegistry()->GetPipeline()->GetClass( &Manager ) );

    Manager->RegisterFile( "Test", "Test.conf" );
    Djinn::Configuration::ConfigFile* const RegisteredConfig = Manager->Find( "Test" );
    const Djinn::Int32 RegisteredHave = RegisteredConfig->ReadInt( "Virgins", "Have", -1 );
    const Djinn::Int32 RegisteredNeed = RegisteredConfig->ReadInt( "Virgins", "Need", -1 );

    // Regex
    Regex::RegExpr RegExpression;
    String Pattern;
    Int32 Position = 0;

    if( RegExpression.Compile( "ab*" ) ) {
        if( RegExpression.FindFirstPattern( "abbc", Position, Pattern ) ) {
            while( RegExpression.FindNextPattern( Position, Pattern ) ) {
                Debug::Console::Print( ( "Found Pattern " + Pattern ).AsCharArray() );
            }
        }
    }

//    String Input( "abbc" );
//
//    String Pattern( "ab*" );
//    RegExpression.Compile( Pattern );
//
//    Int32 Position;
//
//    // Find result.
//    if( RegExpression.FindFirstPattern( Input, Position, Pattern ) ) {
//        // Do something with result.
//        while( RegExpression.FindNextPattern( Position, Pattern ) ) {
//            // Process result.
//            Debug::Console::Print( ( "Found Pattern " + Pattern ).AsCharArray() );
//        }
//    }

    String UnixToWindows = "C:/SomeUser/SomeFile.txt";
    Djinn::Platform::FileSystem::UnixToWindows( UnixToWindows );

    String WindowsToUnix = "C:\\SomeUser\\SomeFile.txt";
    Djinn::Platform::FileSystem::WindowsToUnix( WindowsToUnix );

    Manager->RegisterDirectory( "Config" );
    Djinn::Configuration::ConfigFile* const GameConf = Manager->Find( "Game" );
    const Djinn::Core::String GameName = GameConf->ReadString( "Game", "Name", "Unknown" );


    bool bbreakme = false;
}

int main() {
    Base::Launch( Base::ApplicationInfo().Name( "Fuck off" ), Bindg( _main ) );
    return 0;
}

