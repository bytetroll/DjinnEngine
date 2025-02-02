//
// Created by Dev on 12/24/2017.
//
#pragma once

#include <Core/Buffer.h>
#include "Commands.h"

namespace Djinn::GHA {
    class ScheduledNode {
    public:
        DJINN_INLINE void Flush() {
            name = "";
            dependencies.Clear();
            commands.Clear();
        }

        /// Debug name of this node
        const char* name = "";

        /// Dependencies of this node
        Core::Buffer<ScheduledNode*> dependencies;

        /// Commands executed by this node
        Core::Buffer<Command> commands;
    };
}