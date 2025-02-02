//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>> Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>> GameState_Startup.h
//>>
//>> Startup game state.
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#pragma once

#include <Core/Common.h>

#include "../Framework/IGameState.h"

#include <iostream>

class GameState_Startup : public IGameState {
    // @TODO: Move function definitions to source file once real work starts.  NCY; 11.24.2018.

public:
    void Enter() override {
        std::cout << "Startup" << std::endl;
    }

    void Think( double delta ) override {
    }

    void Pause() override {
    }

    void Unpause() override {
    }

    void Exit() override {
    }
};