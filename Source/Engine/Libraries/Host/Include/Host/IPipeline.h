#pragma once

#include <COM/ID.h>
#include <COM/IUnknown.h>
#include "COM/IInstanceHost.h"
#include "InstanceHost.h"

namespace Djinn {
    namespace Host {
        // Common results
        DJINN_COM_RESULT(PipelineNoClass, true, "Plugin");
        DJINN_COM_RESULT(PipelineNoInterface, true, "Plugin");

        // Interface
        // Hosts instances of classes
        class IPipeline {
        public:
            DJINN_COM_INTERFACEID();

            // Remove class
            virtual COM::Result RemoveClass(const COM::ClassID &id) = 0;

            // Add class
            virtual COM::Result AddClass(const COM::ClassID &id, void *instance) = 0;

            // Get class
            virtual COM::Result GetClass(const COM::ClassID &id, void **out) = 0;

            // Remove interface
            virtual COM::Result RemoveInterface(const COM::InterfaceID &id) = 0;

            // Add interface
            virtual COM::Result AddInterface(const COM::InterfaceID &id, void *instance) = 0;

            // Get interface
            virtual COM::Result GetInterface(const COM::InterfaceID &id, void **out) = 0;

            // Remove class helper
            template<typename CLASS>
            COM::Result RemoveClass(CLASS *) {
                return RemoveClass(CLASS::kCID);
            }

            // Add class helper
            template<typename CLASS>
            COM::Result AddClass(CLASS *instance) {
                return AddClass(CLASS::kCID, instance);
            }

            // Get class helper
            template<typename CLASS>
            COM::Result GetClass(CLASS **out) {
                void* obj;
                if (COM::CheckedResult result = GetClass(CLASS::kCID, &obj)) {
                    return result;
                }
                *out = static_cast<CLASS*>(obj);
                return COM::kOK;
            }

            // Remove interface helper
            template<typename INTERFACE>
            COM::Result RemoveInterface(INTERFACE *interface = nullptr) {
                return RemoveInterface(INTERFACE::kIID);
            }

            // Add interface helper
            template<typename INTERFACE>
            COM::Result AddInterface(INTERFACE *interface) {
                return AddInterface(INTERFACE::kIID, interface);
            }

            // Get interface helper
            template<typename INTERFACE>
            COM::Result GetInterface(INTERFACE **out) {
                void* obj;
                if (COM::CheckedResult result = GetInterface(INTERFACE::kIID, &obj)) {
                    return result;
                }
                *out = static_cast<INTERFACE*>(obj);
                return COM::kOK;
            }
        };
    }
}