//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <Core/Common.h>
#include <Core/Atomic.h>
#include <Core/Types.h>

namespace Djinn {
    namespace Core {
        class ReferenceController {
        public:
            template<typename T>
            ReferenceController(T* object) : object(object) {
                deleter = [](void *ptr) { delete static_cast<T *>(ptr); };
            }

            void *object = nullptr;
            void (*deleter)(void *object) = nullptr;
            Int32 shared = 0;
            Int32 weak = 1;
        };

        enum class ReferenceMode {
            eThreadSafe,
            eSingleThreaded
        };

        /// Developer operations
        /// Must define:
        ///     GetShared
        ///     GetWeak
        ///     IncrementShared()
        ///     DecrementShared()
        ///     IncrementWeak()
        ///     DecrementWeak()
        /// \tparam MODE
        template<ReferenceMode MODE>
        class ReferenceOps;

        template<>
        class ReferenceOps<ReferenceMode::eThreadSafe> {
        public:
            static DJINN_INLINE Int32 GetShared(ReferenceController *controller) {
                return AtomicLoad(controller->shared);
            }

            static DJINN_INLINE Int32 GetWeak(ReferenceController *controller) {
                return AtomicLoad(controller->weak);
            }

            static DJINN_INLINE void IncrementShared(ReferenceController *controller) {
                AtomicIncrement(controller->shared);
            }

            static DJINN_INLINE void DecrementShared(ReferenceController *controller) {
                if (AtomicDecrement(controller->shared) == 0) {
                    // Delete object
                    controller->deleter(controller->object);
                    controller->object = nullptr;

                    // Initial weak reference
                    DecrementWeak(controller);
                }
            }

            static DJINN_INLINE void IncrementWeak(ReferenceController *controller) {
                AtomicIncrement(controller->weak);
            }

            static DJINN_INLINE void DecrementWeak(ReferenceController *controller) {
                if (AtomicDecrement(controller->weak) == 0) {
                    // Controller is no longer referenced
                    delete controller;
                }
            }
        };

        template<>
        class ReferenceOps<ReferenceMode::eSingleThreaded> {
        public:
            static DJINN_INLINE Int32 GetShared(ReferenceController *controller) {
                return controller->shared;
            }

            static DJINN_INLINE Int32 GetWeak(ReferenceController *controller) {
                return controller->weak;
            }

            static DJINN_INLINE void IncrementShared(ReferenceController *controller) {
                controller->shared++;
            }

            static DJINN_INLINE void DecrementShared(ReferenceController *controller) {
                if (--controller->shared == 0) {
                    // Delete object
                    controller->deleter(controller->object);

                    // Initial weak reference
                    DecrementWeak(controller);
                }
            }

            static DJINN_INLINE void IncrementWeak(ReferenceController *controller) {
                controller->weak++;
            }

            static DJINN_INLINE void DecrementWeak(ReferenceController *controller) {
                if (--controller->weak == 0) {
                    // Controller is no longer referenced
                    delete controller;
                }
            }
        };

        template<ReferenceMode MODE>
        class SharedReferenceScope {
        public:
            using Ops = ReferenceOps<MODE>;

            SharedReferenceScope(ReferenceController *controller = nullptr) : controller(controller) {
                if (controller) {
                    Ops::IncrementShared(controller);
                }
            }

            SharedReferenceScope(const SharedReferenceScope& other) : controller(other.controller) {
                if (controller) {
                    Ops::IncrementShared(controller);
                }
            }

            SharedReferenceScope(SharedReferenceScope&& other) : controller(other.controller) {
                other.controller = nullptr;
            }

            SharedReferenceScope& operator=(const SharedReferenceScope& other) {
                if (&other == this) {
                    return *this;
                }

                if (controller) {
                    Ops::DecrementShared(controller);
                }

                if ((controller = other.controller)) {
                    Ops::IncrementShared(controller);
                }
                return *this;
            }

            SharedReferenceScope& operator=(SharedReferenceScope&& other) {
                if (&other == this) {
                    return *this;
                }

                controller = other.controller;
                other.controller = nullptr;
                return *this;
            }

            ~SharedReferenceScope() {
                if (controller) {
                    Ops::DecrementShared(controller);
                }
            }

            ReferenceController *controller;
        };

        template<ReferenceMode MODE>
        class WeakReferenceScope {
        public:
            using Ops = ReferenceOps<MODE>;

            WeakReferenceScope(ReferenceController *controller = nullptr) : controller(controller) {
                if (controller) {
                    Ops::IncrementWeak(controller);
                }
            }

            WeakReferenceScope(const WeakReferenceScope& other) : controller(other.controller) {
                if (controller) {
                    Ops::IncrementWeak(controller);
                }
            }

            WeakReferenceScope(WeakReferenceScope&& other) : controller(other.controller) {
                other.controller = nullptr;
            }

            WeakReferenceScope& operator=(const WeakReferenceScope& other) {
                if (&other == this) {
                    return *this;
                }

                if (controller) {
                    Ops::DecrementWeak(controller);
                }

                if ((controller = other.controller)) {
                    Ops::IncrementWeak(controller);
                }
                return *this;
            }

            WeakReferenceScope& operator=(WeakReferenceScope&& other) {
                if (&other == this) {
                    return *this;
                }

                controller = other.controller;
                other.controller = nullptr;
                return *this;
            }

            ~WeakReferenceScope() {
                if (controller) {
                    Ops::DecrementWeak(controller);
                }
            }

            ReferenceController *controller;
        };
    }
}