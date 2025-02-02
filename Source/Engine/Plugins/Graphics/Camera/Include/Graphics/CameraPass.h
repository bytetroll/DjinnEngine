//
// Created by Dev on 2/24/2018.
//
#pragma once

#include "ICameraPass.h"

namespace Djinn::Graphics {
    class ICameraProcess;
    class ISceneProcess;

    class CameraPass : public ICameraPass {
    public:
        DJINN_COM_CLASS();

        CameraPass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this pass
        /// \return
        COM::Result Initialize(ISceneProcess* worldProcess, ICameraProcess* cameraProcess);

        /// Overrides
                virtual COM::Result Preprocess(PassBuilder builder) override;
        virtual COM::Result Build(PassBuilder builder, double delta) override;
        virtual COM::Result Configure() override;

    private:
        ISceneProcess* sceneProcess;
        ICameraProcess* cameraProcess;
    };
}