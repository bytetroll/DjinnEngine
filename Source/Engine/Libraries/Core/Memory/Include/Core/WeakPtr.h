//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include "SharedPtr.h"

namespace Djinn {
    namespace Core {
        /// Weak pointer
        /// \tparam T
        /// \tparam MODE
        template<typename T, ReferenceMode MODE = ReferenceMode::eThreadSafe>
        class WeakPtr {
            using Ops = ReferenceOps<MODE>;

        public:
            /// Zero constructor
            WeakPtr(std::nullptr_t = nullptr) {}

            /// Construct from reference controller
            WeakPtr(ReferenceController *controller, T* ptr) : scope(controller), object(ptr) {}

            /// Construct from shared pointer
            WeakPtr(const SharedPtr<T, MODE>& shared) : WeakPtr(shared.GetController(), shared.Get()) {}

            /// Pin this weak pointer
            /// \return
            SharedPtr<T, MODE> Pin() const {
                return {scope.controller};
            }

            /// Cast to base type
            /// \return
            operator T*() const {
                return Get();
            }

            /// Member operator
            /// \return
            T *operator->() const {
                auto ptr = Get();
                DJINN_ASSERT(ptr != nullptr, "Attempting to access null weak pointer");
                return ptr;
            }

            /// Value operator
            /// \return
            T *operator*() const {
                return Get();
            }

            /// Get the object
            /// \return
            T *Get() const {
                if (GetSharedCount() == 0) {
                    return nullptr;
                }
                return object;
            }

            /// Check if this weak pointer is valid
            /// \return
            bool IsValid() const {
                return Get() != nullptr;
            }

            /// Validity operator
            /// \return
            operator bool() const {
                return Get() != nullptr;
            }

            /// Comparison
            bool operator ==(const WeakPtr& other) const {
                return object == other.object;
            }

            /// Comparison
            bool operator !=(const WeakPtr& other) const {
                return object != other.object;
            }

            /// Get the weak count
            Int32 GetWeakCount() const {
                if (!object) {
                    return 0;
                }
                return Ops::GetWeak(scope.controller);
            }

            /// Get the shared count
            Int32 GetSharedCount() const {
                if (!object) {
                    return 0;
                }
                return Ops::GetShared(scope.controller);
            }

            /// Get the controller
            /// \return
            ReferenceController *GetController() const {
                return scope.controller;
            }

        private:
            /// Object
            T *object = nullptr;

            /// Developer counter
            WeakReferenceScope<MODE> scope;
        };
    }
}