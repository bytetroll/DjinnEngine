//
// Created by Dev on 2/6/2018.
//
#pragma once

#include <COM/ID.h>

namespace Djinn::Data {
    class IAssetConfiguration {
    public:
        DJINN_COM_INTERFACEID();

        /// Get class id of this configuration
        /// \return
        virtual COM::ClassID GetClassID() const = 0;

        /// Hash this configuration
        /// \return
        virtual USize Hash() = 0;

        /// Create a copy of this configuration
        /// \return
        virtual IAssetConfiguration* Copy() = 0;
    };
}