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
 on 1/8/2018.
//
#pragma once

#include <Async/Detail/AsyncHelper.h>
#include "Info.h"
#include "Framework.h"
#include "JobGroup.h"

namespace Djinn::Async {
    struct Group {
        Group() {
        }

        ~Group() {
            __Yield();
        }

        /// No copy
        Group(const Group&) = delete;
        Group(const Group&&) = delete;
        Group& operator=(const Group&) = delete;

        /// Has this group completed?
        /// \return
        bool HasCompleted() const {
            return group.IsDone();
        }

        /// Wait for this group to complete
        void Wait() const {
            __Yield();
        }

        /// Wait for the result to become available
        /// Should not be called manually
        void __Yield() const {
            if (!group.IsDone()) {
                Detail::GetFramework()->Yield(&group);
            }
        }

        /// Get job group
        /// \return
        JobGroup* GetGroup() {
            return &group;
        }

        /// Increment this group
        /// \return
        void Increment(UInt32 count = 1) {
            group.Increment(count);
        }

    private:
        mutable JobGroup group;
        mutable bool done = false;
    };
}