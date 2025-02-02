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
 on 20/06/2018.
//

#pragma once

#include <COM/Result.h>
#include "EntityID.h"

namespace Djinn::Game {
    class IEntityComponentState;

    class IEntityComponentHost {
    public:
        /// Create a component
        /// \param eid
        /// \param component
        /// \param out
        /// \return
        virtual COM::Result CreateComponent(const EntityComponentReference& ref) = 0;

        /// Destroy a component
        /// \param id
        /// \return
        virtual COM::Result DestroyComponent(const EntityID &id) = 0;

        /// Get the pointer of a component
        /// \param cid
        /// \param out
        /// \return
        virtual COM::Result GetComponentPtr(const EntityID &id, IEntityComponentState **out) = 0;

        /// Enable a component
        /// \param id
        /// \return
        virtual COM::Result Enable(const EntityID &id) = 0;

        /// Disable a component
        /// \param id
        /// \return
        virtual COM::Result Disable(const EntityID &id) = 0;

        /// Apply any deferred changes
        /// \return
        virtual COM::Result ApplyChanges() = 0;
    };
}