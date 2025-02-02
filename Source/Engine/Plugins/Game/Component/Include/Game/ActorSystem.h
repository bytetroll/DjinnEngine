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
 on 8/18/2018.
//
#pragma once

#include <Core/Buffer.h>
#include <Core/IndexArray.h>
#include "IActorSystem.h"
#include "ActorSystemCallback.h"

namespace Djinn::Game {
    class DJINN_EXPORT_PLUGINS_GAME_COMPONENT ActorSystem : public IActorSystem {
    public:
        DJINN_COM_CLASS();

        ActorSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        COM::Result Queue(const DeferredActorEvent &event) override;
        COM::Result CreateActor(IScene* scene, const COM::ClassID &cid, IActor **out) override;
        COM::Result CreateComponent(IScene* scene, const COM::ClassID &cid, IActor* actor, IActorComponent **out) override;
        COM::Result DestroyActor(IActor *actor) override;
        COM::Result DestroyComponent(IActorComponent *com) override;
        COM::Result Register(ActorSystemCallback *callback) override;
        COM::Result Run(StepMode mode, double delta) override;
        COM::Result Configure(Base::PipelineBuilder &builder) override;
        StepModeSet GetStepMode() override;

    private:
        Core::IndexArray<ActorSystemCallback> callbacks;
        Core::Array<ActorSystemCallback*> stagedCallbacks;
        Core::Array<USize> deletedCallbacks;
        Core::Buffer<IActor*> actorDeletions;
        Core::Buffer<IActor*> actorCreations;
        Core::Buffer<IActorComponent*> componentCreations;
        Core::Buffer<IActorComponent*> componentDeletions;
        Core::Buffer<DeferredActorEvent> deferredEvents;
    };
}