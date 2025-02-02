//
// Created by Dev on 2/24/2018.
//
#pragma once

#include "IScenePass.h"

namespace Djinn::Game {
    class IWorld;
}

namespace Djinn::Graphics {
    class IGraphicsSystem;
    class ISceneProcess;
    class IWorldProcess;
    class FXAAUtil;

    class DJINN_EXPORT_PLUGINS_GRAPHICS_SCENE ScenePass : public IScenePass {
    public:
        DJINN_COM_CLASS();

        ScenePass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this pass
        /// \return
        COM::Result Initialize(IWorldProcess* worldProcess, ISceneProcess* uiProcess);

        /// Overrides
        virtual COM::Result Preprocess(PassBuilder builder) override;
        virtual COM::Result Build(PassBuilder builder, double delta) override;
        virtual COM::Result Configure() override;

    private:
        IGraphicsSystem* system;
        ISceneProcess* sceneProcess;
        IWorldProcess* worldProcess;
    };
}