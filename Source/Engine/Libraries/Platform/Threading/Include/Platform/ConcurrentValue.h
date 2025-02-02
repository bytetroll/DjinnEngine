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
 on 3/11/2018.
//
#pragma once

#include <Core/Atomic.h>
#include "SpinMutex.h"
#include "LockGuard.h"

namespace Djinn::Platform {
    /// Lockless spin based concurrent value
    /// \tparam T
    template<typename T>
    struct ConcurrentValue {
    public:
        ConcurrentValue(const T &value = T()) : value(value) {

        }

        /// Get value
        /// \return
        T Get() {
            LockGuard guard(mtx);
            return value;
        }

        /// Set value
        /// \param value
        void Set(const T &value) {
            LockGuard guard(mtx);
            this->value = value;
        }

        /// Implicit cast
        /// \return
        operator T() {
            return Get();
        }

        /// Assign operator
        /// \param value
        /// \return
        ConcurrentValue<T> &operator=(const T &value) {
            Set(value);
            return *this;
        }

    private:
        /// Master value
        T value;

        /// Lock
        SpinMutex mtx;
    };
}