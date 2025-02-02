// Copyright (C) 2021 Djinn Engine Team.  All rights reserved.

#include <Base/Application.h>
#include <Base/ApplicationInfo.h>
#include <Base/DefaultLogs.h>

#include "Common.h"
#include "GameState.h"
#include "GameStateHandler.h"

namespace Tetris {
    using namespace Djinn;

    class GameStateHandler;

    GameState::GameState() : Application(this) {
    }

    GameState::~GameState() {
    }

//    void* GameState::Create(GameStateHandler * parent, Core::String const& name) {
//    }

    void GameState::Destroy() {
        delete this;
    }

    bool GameState::Pause() {
        return true;
    }

    void GameState::Resume() {
    }

    GameState* GameState::Find(Core::String const& name) {
        return parent->Find(name);
    }

    void GameState::Change(GameState* state) {
        return parent->Change(state);
    }

    bool GameState::Push(GameState* state) {
        return parent->Push(state);
    }

    void GameState::Pop() {
        parent->Pop();
    }

    void GameState::Shutdown() {
        parent->Shutdown();
    }

    void GameState::PopAllPush(GameState* state) {
        parent->PopAllPush(state);
    }

    Base::Application* GameState::GetApplicationImpl() const {
        return parent->GetApplication();
    }
}