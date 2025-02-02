//
// Created by Dev on 3/29/2018.
//
#pragma once

#include <Platform/SpinMutex.h>
#include <Platform/LockGuard.h>
#include "ScheduledJob.h"
#include "JobBucket.h"

namespace Djinn::Async {
    struct DJINN_EXPORT_PLUGINS_ASYNC JobGroup {
        ~JobGroup() {
            // Other workers may still be operating on this group, wait for them
            Platform::LockGuard guard(mutex);
        }

        /// Add a scheduled job
        /// \param job
        void Add(const ScheduledJob &job) {
            Platform::LockGuard guard(mutex);
            scheduledJobs.Add(job);
            if (counter == argument) {
                Submit();
            }
        }

        /// Add a yielded job
        /// \param job
        void Add(const YieldedJob &job) {
            Platform::LockGuard guard(mutex);
            yieldedJobs.Add(job);
            if (counter == argument) {
                Submit();
            }
        }

        /// Add an external (untracked) dependency
        /// \param count
        void Increment(UInt32 count = 1) {
            Platform::LockGuard guard(mutex);
            argument += count;
        }

        /// Set the total numer of external (untracked) dependency
        /// \param count
        void Set(UInt32 count) {
            Platform::LockGuard guard(mutex);
            argument = count;
        }

        /// Set the internal counter
        /// \param count
        void SetCounter(UInt32 count) {
            Platform::LockGuard guard(mutex);
            counter = count;
        }

        /// Is this job group done?
        /// \return
        bool IsDone() {
            Platform::LockGuard guard(mutex);
            return counter == argument;
        }

        /// Submit all waiting jobs without checking for condition
        void SubmitEarly() {
            Platform::LockGuard guard(mutex);
            Submit();
        }

        /// Apply a post operation
        /// \param post
        void Apply(const JobPostOp &post) {
            Platform::LockGuard guard(mutex);

            // Apply post argument
            switch (post.op) {
                case CounterOp::eNone:
                    break;
                case CounterOp::eSubtract:
                    counter -= post.argument;
                    break;
                case CounterOp::eAdd:
                    counter += post.argument;
                    break;
            }

            auto counterCpy = counter;
            auto argumentCpy = argument;

            // Verify condition
            bool met = false;
            switch (op) {
                case ConditionalOp::eNone:
                    return;
                case ConditionalOp::eLess:
                    met = counter < argument;
                    break;
                case ConditionalOp::eLessEq:
                    met = counter <= argument;
                    break;
                case ConditionalOp::eMore:
                    met = counter > argument;
                    break;
                case ConditionalOp::eMoreEq:
                    met = counter >= argument;
                    break;
                case ConditionalOp::eEqual:
                    met = counter == argument;
                    break;
                case ConditionalOp::eNotEqual:
                    met = counter != argument;
                    break;
            }

            if (met) {
                Submit();
            }
        }

    private:
        void Submit();

        /// Data mutex
        Platform::SpinMutex mutex;

        /// Counter Op
        Int64 counter = 0;
        Int64 argument = 0;
        ConditionalOp op = ConditionalOp::eMoreEq;

        /// Pending jobs
        JobBucket<ScheduledJob> scheduledJobs;
        JobBucket<YieldedJob> yieldedJobs;
    };
}