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
 on 12/24/2017.
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