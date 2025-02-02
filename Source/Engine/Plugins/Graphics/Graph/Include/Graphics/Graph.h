//
// Created by Dev on 12/26/2017.
//
#pragma once

#include <Core/Pool.h>
#include <Core/Array.h>
#include "IGraph.h"
#include "PassBuilder.h"

namespace Djinn::GHA {
    class ScheduledNode;
}

namespace Djinn::Graphics {
    class Pipeline;
    class RootProcess;
    class PassNode;

    class DJINN_EXPORT_PLUGINS_GRAPHICS_GRAPH Graph : public IGraph {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Graph(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this graph
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual IProcess *GetProcess() override;
        virtual COM::Result Compile(GHA::IScheduler *scheduler, double delta) override;

    private:
        RootProcess* process;
        PassContext passContext;
        Core::LazyObjectPool<PassNode> scheduledNodePool;
        Core::Array<GHA::ScheduledNode*> scheduledGHANodes;
    };
}