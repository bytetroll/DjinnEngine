//
// Created by Dev on 1/17/2018.
//
#pragma once

#include <Game/IStepSystem.h>

namespace Djinn::Game {
    class IComponentSystem;
    class IWorldHost;

    class DJINN_EXPORT_PLUGINS_GAME_WORLD WorldHostSystem : public Game::IStepSystem {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        WorldHostSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize
        /// \param host
        /// \return
        COM::Result Initialize(IWorldHost *host);

        /// Overrides
        virtual COM::Result Run(StepMode mode, double delta) override;
        virtual StepModeSet GetStepMode() override;
        virtual COM::Result Configure(Base::PipelineBuilder& builder) override;

    private:
        IWorldHost *host = nullptr;
    };
}