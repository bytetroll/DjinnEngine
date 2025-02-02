//
// Created by Dev on 10/19/2017.
//
#pragma once

#include <COM/IUnknown.h>
#include "Archive.h"

namespace Djinn::Serialization {
    class ISerializerHost : public COM::TUnknown<ISerializerHost> {
    public:
        DJINN_COM_INTERFACE();

        ISerializerHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Serialize object
        /// \param object
        /// \return
        virtual COM::Result Serialize(Archive& archive, COM::IUnknown* object) = 0;

        /// Serialize object from archive
        /// \param out
        /// \return
        virtual COM::Result Deserialize(Archive& archive, COM::IUnknown** out) = 0;

        /// Serialize object from archive into existing object
        /// \param out
        /// \return
        virtual COM::Result Deserialize(Archive& archive, COM::IUnknown* object) = 0;
    };
}