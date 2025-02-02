//
// Created by Dev on 5/1/2018.
//
#pragma once

#include <Async/Container.h>
#include <Core/HashMap.h>
#include "IPipeline.h"

namespace Djinn::Base {
    class Pipeline : public IPipeline {
    public:
        DJINN_COM_CLASS();

        Pipeline(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void SetConfigurationMode(PipelineConfigurationMode mode) override;
        COM::Result Configure() override;
        COM::Result Register(ISystem *system) override;
        COM::Result Deregister(ISystem *system) override;
        COM::Result AddDependency(ISystem *source, ISystem *dest) override;
        virtual COM::Result RegisterProxy(const COM::ClassID &cid, ISystem *system) override;
        COM::Result GetClass(const COM::ClassID &id, ISystem **out) override;
        COM::Result GetInterface(const COM::InterfaceID &id, ISystem **out) override;
        void RunAsync(Async::Group &group, const Core::Delegate<void(ISystem *)> &delegate) override;

    private:
        Core::Atomic<bool> dirty{false};
        Core::HashMap<COM::ClassID, ISystem*> classes;
        Core::Array<ISystem*> systems;
        Core::Array<Core::Pair<ISystem*, ISystem*>> uniformDependencies;
        PipelineConfigurationMode mode = PipelineConfigurationMode::eIteration;
        Async::Container<ISystem*> container;
        Platform::SpinMutex mutex;
    };
}