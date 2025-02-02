//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>> Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// Engine core.
#include <Core/Common.h>

// Engine application worldHost.
#include <Base/Application.h>
#include <Base/DefaultLogs.h>

#include "Framework/GameStateDriver.h"

#include "States/GameState_Startup.h"
#include "States/GameState_Shutdown.h"

DJINN_NS();

void ApplicationEntry( Base::Application& app ) {
    // Required setup.
    //Always( app.GetRegistry()->LoadPlugins() );

    GameStateDriver* gameStateDriver = new GameStateDriver;
    gameStateDriver->Register( "Startup", new GameState_Startup, true );
    gameStateDriver->Register( "Shutdown", new GameState_Shutdown );

    gameStateDriver->Switch( "Shutdown" );

    delete gameStateDriver;
}

int main( int argc, char** argv ) {
    Base::Launch( Base::ApplicationInfo().Name( "Depths of Gehenna" ), Bindg( ApplicationEntry ) );
    return 0;
}