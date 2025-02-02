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
 on 3/29/2018.
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