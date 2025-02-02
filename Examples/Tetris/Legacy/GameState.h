// Copyright (C) 2021 Djinn Engine Team.  All rights reserved.
#pragma once

#include <Core/String.h>
#include <Core/Property.h>
#include <Base/Application.h>
#include <Base/ApplicationInfo.h>
#include <Base/DefaultLogs.h>

#include "Common.h"

namespace Tetris {
    using namespace Djinn;

    class GameStateHandler;

    class GameState {
    public:
        // Macro registration.
        static void* Manage(GameStateHandler* parent, Core::String const& name);

    public:
        virtual ~GameState();

        void Destroy();

        virtual void Enter() = 0;
        virtual void Exit() = 0;
        virtual bool Pause();
        virtual void Resume();
        virtual void Think(double delta) = 0;

    protected:
        GameState();

        GameState* Find(Core::String const& name);
        void Change(GameState* state);
        bool Push(GameState* state);
        void Pop();
        void Shutdown();
        void PopAllPush(GameState* state);

        // @Note: this property has a special implementation due to forward decl's.
        DJINN_PROPERTYR(GameState, Base::Application*, Application, { return GetApplicationImpl();});

    private:
        Base::Application* GetApplicationImpl() const;

    protected:
        GameStateHandler* parent;

        // Camera, world, etc. here.
    };
}