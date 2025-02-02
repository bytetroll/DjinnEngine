//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 2/28/2018.
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