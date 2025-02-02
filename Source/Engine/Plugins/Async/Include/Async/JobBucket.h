//
// Created by Dev on 3/29/2018.
//
#pragma once

#include <Core/Size.h>
#include "ScheduledJob.h"

namespace Djinn::Async {
    class JobAllocator {

    };

    template<typename T, int STACK_SIZE = 10>
    class JobBucket {
    public:
        ~JobBucket() {
            Reset();
        }

        /// Add job to bucket
        /// \param job
        void Add(const T &job) {
            auto index = size++;
            if (index + 1 >= STACK_SIZE) {
                DJINN_THROW std::exception();
            } else {
                stack[index] = job;
            }
        }

        /// Get pointer of bucket
        /// \return
        T *Ptr() {
            if (size > STACK_SIZE) {
                return ptr;
            } else {
                return stack;
            }
        }

        /// Reset this bucket
        void Reset() {
            size = 0;
        }

        /// Get size of this bucket
        /// \return
        UInt32 Size() {
            return static_cast<UInt32>(size);
        }

    private:
        T stack[STACK_SIZE];
        T *ptr = nullptr;
        USize size = 0;
    };
}