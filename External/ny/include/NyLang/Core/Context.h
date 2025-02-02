//
// Created by Dev on 5/5/2018.
//
#pragma once

#include "LazyAllocator.h"

namespace NyLang {
    class Context {
    public:
        ~Context() {

        }

        /// Create new from instance
        /// \tparam T
        /// \param from
        /// \return
        /*template<typename T>
        T* New(const T& from) {
            auto ptr = allocator.AllocateUninitialized<T>();
            new (ptr) T(from);
            return ptr;
        }*/

        /// Create new from instance
        /// \tparam T
        /// \param from
        /// \return
        template<typename T, typename... A>
        T* New(A&&... args) {
            auto ptr = allocator.AllocateUninitialized<T>();
            new (ptr) T(args...);
            return ptr;
        }

        /// Create new to destination
        /// \tparam T
        /// \param to
        /// \return
        /*template<typename T>
        T* New(T*& to) {
            new (to = allocator.AllocateUninitialized<T>()) T();
            return to;
        }*/

        /// Create new by array
        /// \tparam T
        /// \param count
        /// \return
        template<typename T>
        T* NewArray(std::size_t count) {
            auto ptr = allocator.AllocateUninitialized<T>(count);
            return ptr;
        }

    private:
        LazyAllocator<> allocator;
    };
}