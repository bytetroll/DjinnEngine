//
// Created by Dev on 12/24/2017.
//
#pragma once

#include "IndexArray.h"

namespace Djinn::Core {
    /// Proxy context
    /// \tparam T - Contained type
    template<typename T>
    struct ProxyContext {
        /// Add a value to this context
        /// \param value
        /// \return
        USize Add(T *value) {
            stack.Add(value);
            return stack.Size() - 1;
        }

        /// Remove a value from this context
        /// \param value
        void Remove(T *value) {
            stack.RemoveValue(value);
        }

        // Current stack
        Core::IndexArray<T *> stack;
    };

    /// Proxy wrapper
    /// \tparam T - Contained type
    /// \tparam U - Proxy contained type (defaulted T), will be casted to T
    template<typename T, typename U = T>
    class Proxy {
    public:
        Proxy() = default;
        Proxy(ProxyContext<U> *context, USize index) : context(context), index(index) {

        }

        /// Member accessor
        /// \return
        T *operator->() const {
            return context ? ((T*)context->stack[index]) : nullptr;
        }

    private:
        ProxyContext<U> *context;
        USize index;
    };
}