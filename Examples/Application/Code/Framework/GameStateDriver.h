//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>> Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>> GameStateDriver.h
//>>
//>> Game state driver responsible for game state operations.  All states and state operations should be managed
//>> through this driver.
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#pragma once

#include <Core/Common.h>
#include <Core/Array.h>
#include <Core/String.h>

#include "IGameState.h"

DJINN_NS();

class GameStateDriver {
private:
    struct StateCacheSnapshot {
    public:
        StateCacheSnapshot() : name( "" ), state( nullptr ) {
        }

        StateCacheSnapshot( const Core::String& name, IGameState* state ) : name( name ), state( state ) {
        }

        ~StateCacheSnapshot() {
            // I see a memory leak here from not cleaning up the state pointer.  By nature of the game state driver
            // I don't see this being a huge issue, but it's something that will need to be looked into.  For now
            // We will just clean up each individual state manually at the end of the application.  NCY; 11.24.2018.
        }

    public:
        Core::String name;
        IGameState* state;
    };

public:
    explicit GameStateDriver();
    GameStateDriver( Core::String& name, IGameState* state );
    ~GameStateDriver();

    void Register( const Core::String& name, IGameState* state, bool asStartingState = false );
    void Deregister( Core::String& name, bool restortToNext = false );

    IGameState* Find( Core::String& name );

    void Switch( const Core::String& name, bool pauseActive = true );

    void Think( double delta );

private:
    StateCacheSnapshot* LookupSnapshot( const Core::String& name );

private:
    StateCacheSnapshot* activeState;
    Core::Array< StateCacheSnapshot* > activeStateCache;
};