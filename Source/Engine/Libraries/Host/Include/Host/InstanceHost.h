#pragma once

#include <COM/Result.h>
#include <COM/IInstanceHost.h>
#include <Core/Assert.h>
#include <Core/Atomic.h>
#include <Core/TypeName.h>

namespace Djinn {
    namespace Host {
        class IRegistry;

        namespace Detail {
            extern void LiveInstanceWarning(const char *name, Int64 count);
        }

        // Default worldHost, may not be sufficient
        template<class T>
        class InstanceHost : public COM::IInstanceHost {
        public:
            ~InstanceHost() {
                Int64 count = liveInstances.Load();
                if (count != 0) {
                    Detail::LiveInstanceWarning(GetStaticTypeName<T>(), count);
                }
            }

            /// Override
            COM::Result CreateInstance(IRegistry *registry, COM::IUnknown *outer, COM::IUnknown **out) override {
                // Create
                if (!(*out = static_cast<COM::IUnknown *>(new T(registry, outer, this)))) {
                    return COM::kOutOfMemory;
                }

                // Track
                liveInstances++;

                // OK
                return COM::kOK;
            }

            /// Override
            COM::Result DestroyInstance(COM::IUnknown *com) override {
                // ...
                delete com;

                // Track
                liveInstances--;

                // OK
                return COM::kOK;
            }

            Core::AtomicCounter liveInstances{0};
        };
    }
}