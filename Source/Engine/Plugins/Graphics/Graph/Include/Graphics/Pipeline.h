//
// Created by Dev on 12/26/2017.
//
#pragma once

#include <Core/Array.h>
#include <Core/HashMap.h>
#include <Core/Buffer.h>
#include "IPipeline.h"

namespace Djinn::Graphics {
    class Pipeline : public IPipeline {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Pipeline(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
                virtual COM::Result Build(PassBuilder builder, double delta) override;
        virtual COM::Result Register(IPass *pass) override;
        virtual COM::Result Deregister(IPass *pass) override;
        virtual COM::Result GetPass(const COM::ClassID &cid, IPass** out) override;
        virtual COM::Result GetPass(const COM::InterfaceID &iid, IPass** out) override;
        virtual COM::Result AddDependency(IPass *dependent, IPass *target) override;

    private:
        COM::Result Configure();

        struct PassState {
            IPass* pass = nullptr;
            PassBuilder builder;
            Core::Buffer<PassState*> dependencies;
        };

        bool isDirty = false;
        Core::Array<PassState*> passStates;
        Core::ObjectPool<PassState> statePool;
        Core::HashMap<COM::ClassID, IPass*> passMap;
    };
}