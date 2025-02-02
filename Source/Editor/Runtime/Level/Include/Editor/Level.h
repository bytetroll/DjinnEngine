//
// Created by Dev on 6/28/2018.
//
#pragma once

#include "ILevel.h"

namespace Djinn::Host {
    class Pipeline;
}

namespace Djinn::Editor {
    class CommandHost;
    class SceneCamera;

    class Level : public ILevel {
    public:
        DJINN_COM_CLASS();

        Level(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this level
        /// \return
        COM::Result Initialize();

        /// Overrides
        ICommandHost *GetCommandHost() override;
        Game::IWorld *GetGameWorld() override;
        Host::IPipeline *GetPipeline() override;

    private:
        CommandHost* commandHost = nullptr;
        Game::IWorld *gameWorld = nullptr;
        SceneCamera* navigator = nullptr;
        Host::Pipeline *pipeline = nullptr;
    };
}