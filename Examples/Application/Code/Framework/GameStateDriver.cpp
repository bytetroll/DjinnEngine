//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>> Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>> GameStateDriver.cpp
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include <Core/Common.h>
#include <Core/Array.h>
#include <Core/String.h>

#include "IGameState.h"
#include "GameStateDriver.h"

GameStateDriver::GameStateDriver() : activeState( nullptr ) {
    activeStateCache.Clear();
}

// It is assumed that if this constructor is called that the state that is passed as an argument is a starting state.
// Thus, we will both add it and make it the active state.  NCY; 11.24.2018.
GameStateDriver::GameStateDriver( Core::String& name, IGameState* state ) {
    Register( name, state, true );
}

GameStateDriver::~GameStateDriver() {
    for( auto iter : activeStateCache ) {
        delete iter;
    }
}

void GameStateDriver::Register( const Core::String& name, IGameState* state, bool asStartingState ) {
    activeStateCache.Add( new StateCacheSnapshot( name, state ) );
    if( asStartingState ) {
        activeState = activeStateCache.First();
        activeState->state->Enter();
    }
}

void GameStateDriver::Deregister( Core::String &name, bool resortToNext ) {
    if( activeState->name == name ) {
        activeState->state->Exit();
    }

    activeStateCache.Remove( activeStateCache.Start() );

    activeState = resortToNext ? activeStateCache.First() : nullptr;
}

IGameState* GameStateDriver::Find( Core::String& name ) {
    for( auto iter : activeStateCache ) {
        if( iter->name == name ) {
            return iter->state;
        }
    }
    return nullptr;
}

void GameStateDriver::Switch( const Core::String& name, bool pauseActive ) {
    if( activeState != nullptr ) {
        if( pauseActive ) {
            activeState->state->Pause();
        } else {
            activeState->state->Exit();
        }
    }

    activeState = LookupSnapshot( name );
    activeState->state->Enter();
}

GameStateDriver::StateCacheSnapshot* GameStateDriver::LookupSnapshot( const Core::String& name ) {
    for( auto iter : activeStateCache ) {
        if( iter->name == name ) {
            return iter;
        }
    }
    return nullptr;
}

void GameStateDriver::Think( double delta ) {
    if( activeState != nullptr ) {
        activeState->state->Think( delta );
    }
}