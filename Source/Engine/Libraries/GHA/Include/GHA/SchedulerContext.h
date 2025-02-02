//
// Created by Dev on 1/23/2018.
//
#pragma once

#include <Core/Size.h>
#include <Core/HashMap.h>
#include <Core/Buffer.h>
#include "Commands.h"

namespace Djinn::GHA {
    class ScheduledNode;

    struct SchedulerImport {
        ScheduledNode* node;
        ResourceCommand command;
    };

    struct SchedulerContext {
        /// Reset this context
        void Reset() {
            uid = 0;
            resources.Clear();
            constantBuffer.Clear();
            imported.Clear();
        }

        /// Create a new unique id
        /// \return
        USize CreateUID() {
            return uid++;
        }

        /// Create an imported uid
        /// \param com
        /// \return
        bool CreateImportedUID(void* com, USize& out, USize count = 1) {
            out = imported[com];
            if (out == 0) {
                out = uid;
                uid += count;
                return true;
            }
            return false;
        }

        USize GetConstantOffset(void* buffer, USize size) {
            USize offset = constantBuffer.Size();
            constantBuffer.Resize(constantBuffer.Size() + size);
            Core::Memory::Copy(&constantBuffer[offset], buffer, size);
            return offset;
        }

        /// Unique id
        USize uid = 0;

        /// Imported interfaces
        Core::HashMap<void *, USize> imported;

        /// Resource commands
        Core::Buffer<SchedulerImport> resources;

        /// Constant data buffer
        Core::Buffer<char> constantBuffer;
    };
}