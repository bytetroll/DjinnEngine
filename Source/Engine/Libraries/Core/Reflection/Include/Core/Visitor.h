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
 on 9/5/2018.
//
#pragma once

#include "Delegate.h"

namespace Djinn::Core {
    /// Base visitor interface
    template<typename T>
    class IVisitor;

    template<typename R, typename... A>
    class IVisitor<R(A...)> {
    public:
        virtual R Visit(A... args) = 0;
    };

    /// Visitor helper
    template<typename T>
    struct Visitor;

    /// Visitor helper
    template<typename R, typename... A>
    struct Visitor<R(A...)> {
    public:
        /// Create from visitor
        /// \param interface
        Visitor(IVisitor<R(A...)>* interface = nullptr) : interface(interface) {

        }

        /// Create from delegate
        /// \param delegate
        Visitor(const Delegate<R(A...)>& delegate) : delegate(delegate) {

        }

        /// Create from lambda function
        /// \param instance
        template<
                typename F,
                typename = typename EnableIf<!std::is_function<F>::value && !std::is_same_v<std::decay_t<F>, class DynamicDelegate> && !IsDelegate<F>::kValue>::Type,
                typename = typename EnableIf<std::is_invocable_v<F, A...>>::Type
        >
        Visitor(const F &&instance) : delegate(instance) {
            // ...
        }

        /// Visit the visitor
        /// \param args
        /// \return
        R Visit(A... args) const {
            if (interface) {
                return interface->Visit(args...);
            } else {
                return delegate.Invoke(args...);
            }
        }

        /// Visit the visitor
        /// \param args
        /// \return
        R operator()(A... args) const {
            return Visit(args...);
        }

    private:
        IVisitor<R(A...)>* interface = nullptr;
        Delegate<R(A...)> delegate;
    };
}