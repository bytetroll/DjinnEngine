//
// Created by Dev on 10/18/2017.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Serialization {
    class ISerializer;

    // Common results
    DJINN_COM_RESULT(NoSerializer, true, "Data");

    class ISerializerRegistry : public COM::TUnknown<ISerializerRegistry> {
    public:
        DJINN_COM_INTERFACE();

        ISerializerRegistry(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Add a serializer with no attachments
        /// \param serializer
        /// \return
        virtual COM::Result AddSerializer(ISerializer *serializer) = 0;

        /// Add a serializer mapped to id
        /// \param id - Class ID the serializer handles
        /// \param serializer
        /// \return
        virtual COM::Result AddSerializer(const COM::ClassID& id, ISerializer* serializer) = 0;

        /// Add a serializer mapped to id
        /// \param id - Class ID the serializer handles
        /// \param serializer
        /// \return
        virtual COM::Result AddSerializer(const COM::ClassID& id, const COM::ClassID& serializerID) = 0;

        /// Remove a serializer from id
        /// \param id
        /// \return
        virtual COM::Result RemoveSerializer(const COM::ClassID& id) = 0;

        /// Get serializer from id
        /// \param id
        /// \return
        virtual COM::Result GetSerializer(const COM::ClassID& id, ISerializer** out) = 0;

        /// Get serializer with id
        /// \param id
        /// \return
        virtual COM::Result GetSerializerWithID(const COM::ClassID& id, ISerializer** out) = 0;
    };
}