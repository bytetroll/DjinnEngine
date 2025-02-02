//
// Created by Dev on 10/18/2017.
//
#pragma once

#include <COM/IUnknown.h>
#include "Archive.h"

namespace Djinn::Serialization {
    class ISerializer : public COM::TUnknown<ISerializer> {
    public:
        DJINN_COM_INTERFACE();

        ISerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Serialize supplied object
        /// \param object
        /// \return
        virtual COM::Result Serialize(Archive &archive, COM::IUnknown *object) = 0;

        /// Deserialize from archive
        /// \param object
        /// \return
        virtual COM::Result Deserialize(Archive& archive, COM::IUnknown** object) = 0;

        /// Deserialize from archive into existing object
        /// \param object
        /// \return
        virtual COM::Result Deserialize(Archive& archive, COM::IUnknown* object) = 0;
    };
}