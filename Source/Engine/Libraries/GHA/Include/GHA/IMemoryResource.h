#pragma once

#include <Core/Optional.h>
#include "MemoryBounds.h"
#include "IResource.h"

namespace Djinn {
    namespace GHA {
        // Base type for resource types
        class IMemoryResource : public IResource {
        public:
            DJINN_COM_INTERFACE();

            IMemoryResource(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IResource(registry, outer, host) {

            }

            /// Map this resource
            /// \param out
            /// \param bounds
            /// \return
            virtual COM::Result Map(void** out, const Core::Optional<GHA::MemoryBounds>& bounds = {}) = 0;

            /// Unmap this resource
            /// \param ptr
            /// \return
            virtual COM::Result Unmap(void* ptr) = 0;

            /// Get size of this resource
            /// \return
            virtual USize GetSize() = 0;

            /// Mark this memory resource as initialized
            void MarkAsInitialized(UInt32 index) {
                initialized[index] = true;
            }

            /// Mark this memory resource as uninitialized
            void MarkAsUninitialized(UInt32 index) {
                initialized[index] = false;
            }

            /// Mark this memory resource as initialized
            void MarkAllAsInitialized() {
                for (UInt32 i = 0; i < initializationCount; i++) {
                    initialized[i] = true;
                }
            }

            /// Check if this resource is initialized
            /// \return
            bool IsInitialized(UInt32 index) {
                return initialized[index].Load();
            }

            /// Check if this resource is initialized
            /// \return
            bool IsInitialized() {
                for (UInt32 i = 0; i < initializationCount; i++) {
                    if (!initialized[i].Load()) {
                        return false;
                    }
                }
                return true;
            }

        protected:
            UInt32 initializationCount = 1;

        private:
            Core::Atomic<bool> initialized[16];
        };
    }
}