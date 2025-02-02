//
// Created by Dev on 3/11/2018.
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