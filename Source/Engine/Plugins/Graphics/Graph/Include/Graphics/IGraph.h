//
// Created by Dev on 12/26/2017.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::GHA {
    class IScheduler;
}

namespace Djinn::Graphics {
    class IProcess;

    class IGraph : public COM::TUnknown<IGraph> {
    public:
        DJINN_COM_INTERFACE();

        IGraph(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Get the root process
        /// \return
        virtual IProcess* GetProcess() = 0;

        /// Compile all registered passes into a scheduler
        /// \param scheduler
        /// \return
        virtual COM::Result Compile(GHA::IScheduler* scheduler, double delta) = 0;
    };
}