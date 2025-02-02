//
// Created by Dev on 2/24/2018.
//
#pragma once

#include "IUIPass.h"

namespace Djinn::Game {
    class IWorld;
}

namespace Djinn::Graphics {
    class IGraphicsSystem;
    class IWorldProcess;
    class IUIProcess;

    class DJINN_EXPORT_PLUGINS_GRAPHICS_UI UIPass : public IUIPass {
    public:
        DJINN_COM_CLASS();

        UIPass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this pass
        /// \return
        COM::Result Initialize(Game::IWorld *world, IWorldProcess *worldProcess, IUIProcess *uiProcess);

        /// Overrides
        virtual COM::Result Preprocess(PassBuilder builder) override;
        virtual COM::Result Build(PassBuilder builder, double delta) override;
        virtual COM::Result Configure() override;

    private:
        IGraphicsSystem *system;
        IWorldProcess *worldProcess;
        IUIProcess *uiProcess;
    };
}