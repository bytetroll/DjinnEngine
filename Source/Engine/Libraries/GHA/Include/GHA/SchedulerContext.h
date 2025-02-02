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
 on 1/23/2018.
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