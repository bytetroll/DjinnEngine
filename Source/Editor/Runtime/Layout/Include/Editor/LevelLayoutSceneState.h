//
// Created by Dev on 6/28/2018.
//
#pragma once

#include "ILevelLayoutSceneState.h"

namespace Djinn::Host {
    class Pipeline;
}

namespace Djinn::Editor {
    class LevelLayoutSceneState : public ILevelLayoutSceneState {
    public:
        DJINN_COM_CLASS();

        LevelLayoutSceneState(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this scene
        /// \param level
        /// \return
        COM::Result Initialize(ILevel* level);

        /// Overrides
        ILevel* GetLevel() override;
        Host::IPipeline *GetPipeline() override;

    private:
        ILevel* level = nullptr;
        Host::Pipeline* pipeline = nullptr;
    };
}