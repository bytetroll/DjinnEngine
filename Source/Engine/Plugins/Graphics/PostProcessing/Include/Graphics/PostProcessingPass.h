//
// Created by Dev on 2/24/2018.
//
#pragma once

#include "IPostProcessingPass.h"

namespace Djinn::Game {
    class IWorld;
}

namespace Djinn::Graphics {
    class IGraphicsSystem;
    class ICameraProcess;
    class IPostProcessingProcess;

    class DJINN_EXPORT_PLUGINS_GRAPHICS_POSTPROCESSING PostProcessingPass : public IPostProcessingPass {
    public:
        DJINN_COM_CLASS();

        PostProcessingPass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this pass
        /// \return
        COM::Result Initialize(Game::IWorld* world, ICameraProcess* cameraProcess, IPostProcessingProcess* lightingProcess);

        /// Overrides
        virtual COM::Result Preprocess(PassBuilder builder) override;
        virtual COM::Result Build(PassBuilder builder, double delta) override;
        virtual COM::Result Configure() override;

    private:
        IGraphicsSystem* system;
        ICameraProcess* cameraProcess;
        IPostProcessingProcess* lightingProcess;
    };
}