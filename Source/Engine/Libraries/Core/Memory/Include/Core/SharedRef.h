//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <new>
#include "RefCounter.h"

namespace Djinn {
    namespace Core {
        /// Shared reference
        /// \tparam T
        /// \tparam MODE
        template<typename T, ReferenceMode MODE = ReferenceMode::eThreadSafe>
        class SharedRef {
            using Ops = ReferenceOps<MODE>;

        public:
            /// Construct from reference controller
            SharedRef(ReferenceController *controller, T* ptr) : scope(controller), object(ptr) {}

            /// Cast to base type
            /// \return
            operator T*() const {
                return Get();
            }

            /// Member operator
            /// \return
            T *operator->() const {
                return object;
            }

            /// Value operator
            /// \return
            T *operator*() const {
                return object;
            }

            /// Get the object
            /// \return
            T *Get() const {
                return object;
            }

            /// Check if this shared pointer is valid
            /// \return
            bool IsValid() const {
                return object != nullptr;
            }

            /// Validity operator
            /// \return
            operator bool() const {
                return object != nullptr;
            }

            /// Comparison
            bool operator ==(const SharedRef& other) const {
                return object == other.object;
            }

            /// Comparison
            bool operator !=(const SharedRef& other) const {
                return object != other.object;
            }

            /// Get the shared count
            Int32 GetSharedCount() const {
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
            SharedReferenceScope<MODE> scope;
        };

        /// Cast a shared pointer
        /// \param ptr
        /// \return
        template<typename T, typename U, ReferenceMode MODE>
        static DJINN_INLINE SharedRef<T, MODE> Cast(const SharedRef<U, MODE> &ptr) {
            return SharedRef<T, MODE>(ptr->GetController(), static_cast<T*>(ptr.Get()));
        }

        /// Create a shared pointer
        /// \param ptr
        /// \return
        template<typename T, ReferenceMode MODE = ReferenceMode::eThreadSafe, typename... A>
        static DJINN_INLINE SharedRef<T, MODE> MakeRef(A&&... args) {
            /*// Allocate combined object
            auto alloc = new char[sizeof(T) + sizeof(ReferenceController)];

            // Allocate object
            auto obj = new (reinterpret_cast<T*>(alloc)) T(args...);

            // Allocate controller
            auto controller = new (reinterpret_cast<ReferenceController*>(alloc + sizeof(T))) ReferenceController(obj);

            // ...
            return {controller, obj};*/
            auto obj = new T(args...);
            return{new ReferenceController(obj), obj};
        }
    }
}