//
// Created by Dev on 9/5/2018.
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