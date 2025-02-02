//
// Created by Dev on 12/26/2017.
//
#pragma once

#include <GHA/Commands.h>
#include <Core/Pair.h>
#include <GHA/ScheduledNode.h>
#include "DependencyMode.h"

namespace Djinn::Graphics {
    class PassNode {
    public:
        PassNode() {
            node = new GHA::ScheduledNode();
        }

        void Flush() {
            node->Flush();
            syncNode = nullptr;
            isInvalid = false;
        }

        // Dependency mode
        DependencyMode dependencyMode = DependencyMode::eImplicit;

        // Backend node of this pass node
        GHA::ScheduledNode* node = nullptr;

        // Invalid node?
        bool isInvalid = false;

        // Synchronization node of this pass node
        PassNode* syncNode = nullptr;
    };
}
