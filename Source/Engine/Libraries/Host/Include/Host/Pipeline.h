#pragma once

#include "IPipeline.h"
#include <Platform/Library.h>
#include <Core/HashMap.h>

namespace Djinn {
    namespace Host {
        class Pipeline : public IPipeline {
        public:
            DJINN_COM_CLASSID();

            virtual COM::Result RemoveClass(const COM::ClassID &id) override;
            virtual COM::Result AddClass(const COM::ClassID &id, void *instance) override;
            virtual COM::Result GetClass(const COM::ClassID &id, void **out) override;
            virtual COM::Result RemoveInterface(const COM::InterfaceID &id) override;
            virtual COM::Result AddInterface(const COM::InterfaceID &id, void *instance) override;
            virtual COM::Result GetInterface(const COM::InterfaceID &id, void **out) override;

        private:
            Core::HashMap<COM::ClassID, void*> classes;
            Core::HashMap<COM::InterfaceID, void*> interfaces;
        };
    }
}