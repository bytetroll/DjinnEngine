//
// Created by Dev on 2/28/2018.
//
#pragma once

#include <Core/Array.h>
#include <Host/IPipeline.h>
#include <Async/Container.h>
#include <Core/HashMap.h>
#include "IWorld.h"

namespace Djinn::Host {
    class Pipeline;
}

namespace Djinn::Base {
    class Pipeline;
}

namespace Djinn::Game {
    class TransformComponent;

    class World : public IWorld {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        World(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize
        /// \param config
        /// \return
        COM::Result Initialize(Data::WorldConfig *config);

        /// Overrides
        COM::Result CreateScene(COM::InterfaceID iid, ISceneState* state, IScene **out) override;
        void Register(IWorldPackage *package) override;
        COM::Result Register(IWorldSystem *system) override;
        COM::Result Deregister(IWorldSystem *system) override;
        void RegisterSceneType(COM::InterfaceID iid, COM::ClassID cid) override;
        Base::IPipeline *GetSystemPipeline() override;
        Data::WorldConfig *GetConfig() override;
        Host::IPipeline *GetPipeline() override;
        COM::Result Run(StepMode mode, double delta) override;
        Core::ArrayView<IScene *> GetScenes() override;
        COM::Result Configure(Base::PipelineBuilder &builder) override;
        StepModeSet GetStepMode() override;

    private:
        // All systems
        Base::Pipeline* systemPipeline;
        Data::WorldConfig *config = nullptr;
        Host::Pipeline *pipeline;

        Core::Array<IWorldPackage*> packages;
        Core::HashMap<COM::InterfaceID, COM::ClassID> sceneTypes;

        /// All scenes
        Core::Array<IScene*> scenes;
    };
}