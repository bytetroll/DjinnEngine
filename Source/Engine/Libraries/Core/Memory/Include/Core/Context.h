//
// Created by Dev on 5/5/2018.
//
#pragma once

#include "LazyAllocator.h"

namespace Djinn::Memory {
    class Context {
    public:
        ~Context() {

        }

        /// Create new from instance
        /// \tparam T
        /// \param from
        /// \return
        template<typename T>
        T* New(const T& from) {
            auto ptr = allocator.AllocateUninitialized<T>();
            new (ptr) T(from);
            return ptr;
        }

        /// Create new to destination
        /// \tparam T
        /// \param to
        /// \return
        template<typename T>
        T* New(T*& to) {
            new (to = allocator.AllocateUninitialized<T>()) T();
            return to;
        }

        /// Create new by array
        /// \tparam T
        /// \param count
        /// \return
        template<typename T>
        T* NewArray(USize count) {
            auto ptr = allocator.AllocateUninitialized<T>(count);
            return ptr;
        }

    private:
        LazyAllocator<> allocator;
    };
}