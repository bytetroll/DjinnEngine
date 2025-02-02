//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>> Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>> IGameState.h
//>>
//>> Interface for any game state class to be consumed by the game state driver.  IGameState represents a finite
//>> state that a game can reside in -- i.e., startup, play, etc.
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#pragma once

#include <Core/Common.h>

class IGameState {
public:
    virtual void Enter() = 0;
    virtual void Think( double delta ) = 0;
    virtual void Pause() = 0;
    virtual void Unpause() = 0;
    virtual void Exit() = 0;

    bool CurrentlyPaused() {
        return currentlyPaused;
    }

protected:
    bool currentlyPaused;
};