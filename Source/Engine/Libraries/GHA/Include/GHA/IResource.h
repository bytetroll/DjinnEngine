#pragma once

#include <COM/IUnknown.h>
#include <Core/Optional.h>
#include <Core/Atomic.h>
#include <Core/Template/Limits.h>
#include "MemoryBounds.h"

namespace Djinn {
    namespace GHA {
        // Base type for resource types
        class IResource : public COM::TUnknown<IResource> {
        public:
            DJINN_COM_INTERFACE();

            IResource(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

            /// Get resource iid
            /// \return
            virtual COM::InterfaceID GetIID() = 0;

            /// Get unique id
            /// \return
            USize GetUID() const {
                return uid;
            }

            /// Schedule a lock, returns false if not successful
            /// \return
            bool ScheduleLock(USize userdata) {
                if (lock.CmpExchange(false, true)) {
                    usageCount = Core::LimitsOf<Int64>::kMin;
                    lockUserdata = userdata;
                    return true;
                }
                return false;
            }

            /// Release a lock, must be locked otherwise undefined behaviour
            void ReleaseLock() {
                lock = false;
                usageCount = 0;
            }

            /// Get the current usage count
            /// \return
            Int64 GetUsageCount() {
                Int64 count = usageCount.Load();
                if (count < 0) {
                    return 0;
                }
                return count;
            }

            /// Increment the usage count, returns false if not successful
            /// \return
            bool AcquireUsage() {
                return ++usageCount > 0;
            }

            /// Decrement the usage count
            void RemoveUsage() {
                Int64 c = --usageCount;
                DJINN_ASSERT(c != -1);
            }

            /// Is this resource locked?
            /// \return
            bool Locked() {
                return lock.Load();
            }

            /// Get lock userdata
            /// \return
            USize GetLockUserdata() const {
                return lockUserdata;
            }

        protected:
            USize uid;
            Core::AtomicCounter usageCount{0};
            Core::Atomic<bool> lock{false};
            USize lockUserdata = 0;
        };
    }
}