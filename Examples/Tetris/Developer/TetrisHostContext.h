//
// Created by nathan on 1/12/21.
//

#pragma once

#include <Base/Application.h>
#include <Core/Timer.h>
#include <Host/IPipeline.h>
#include <Platform/User.h>
#include <Game/IGameSystem.h>
#include <Game/IWorldHost.h>
#include <Game/IWorld.h>
#include <Game/IInputSystem.h>
#include <Game/IScene.h>
#include <Game/IActorSystem.h>
#include <Graphics/IGraphicsSystem.h>
#include <Graphics/IOutputHost.h>
#include <Graphics/Output.h>
#include <Data/IAssetHost.h>
#include <Data/WorldConfig.h>
#include <UI/IUIInputSystem.h>

#include "Code/Common.h"

struct TetrisHostContext {
public:
    Application* app  nullptr;
    Timer* timer = nullptr;
    WorldConfig* config;

    // Interfaces
    IPipeline* pipeline = nullptr;
    IGameSystem* game = nullptr;
    IGraphicsSystem* video = nullptr;
    IWorldHost* worldHost = nullptr;
    IWorld* world = nullptr;
    IOutputHost* output = nullptr;
    IAssetHost* asset = nullptr;
    IInputSystem* input = nullptr;
    IScene* scene = nullptr;
    IActorSystem* actor = nullptr;
    IOutput* sceneOutput = nullptr;

};

// @TODO: Finish implementing the context.