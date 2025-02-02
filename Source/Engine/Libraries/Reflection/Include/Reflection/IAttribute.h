//
// Created by Dev on 9/5/2018.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Reflection {
    class IAttribute {
    public:
        DJINN_COM_INTERFACEID();

        /// Invoked when attached to a container
        /// \param container
        virtual void Attached(COM::IQueryable* container) { };

        /// Get the class id of the top annotation
        /// \return
        virtual COM::ClassID GetClassID() = 0;
    };
}