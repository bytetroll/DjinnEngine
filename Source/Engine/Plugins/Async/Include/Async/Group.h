//
// Created by Dev on 1/8/2018.
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