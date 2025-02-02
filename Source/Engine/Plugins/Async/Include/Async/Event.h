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
    struct Event {
        Event() {
            Reset();
        }

        /// Reset this event, submits any waiting jobs
        void Reset() {
            group.SubmitEarly();
            group.Set(1);
            group.SetCounter(0);
        }

        /// Signal this event
        void Signal() {
            JobPostOp op;
            op.argument = 1;
            op.op = CounterOp::eAdd;
            group.Apply(op);
        }

        /// Has this event completed?
        /// \return
        bool HasCompleted() const {
            return group.IsDone();
        }

        /// Wait for this event to complete
        void Wait() {
            __Yield();
            Reset();
        }

        /// Wait for this event to complete without resetting
        void WaitRepeat() const {
            __Yield();
        }

        /// Wait for the result to become available
        /// Should not be called manually
        void __Yield() const {
            if (!HasCompleted()) {
                Detail::GetFramework()->Yield(&group);
            }
        }

    private:
        mutable JobGroup group;
    };
}