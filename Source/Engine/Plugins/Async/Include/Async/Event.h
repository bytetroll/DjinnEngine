//
// Created by Dev on 1/8/2018.
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