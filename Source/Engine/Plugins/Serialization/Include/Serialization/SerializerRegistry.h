//
// Created by Dev on 10/18/2017.
//
#pragma once

#include <Core/HashMap.h>
#include "ISerializerRegistry.h"

namespace Djinn::Serialization {
    class DJINN_EXPORT_PLUGINS_SERIALIZATION SerializerRegistry : public ISerializerRegistry {
    public:
        DJINN_COM_CLASS();

        SerializerRegistry(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        virtual COM::Result AddSerializer(ISerializer *serializer) override;
        virtual COM::Result AddSerializer(const COM::ClassID &id, ISerializer *serializer) override;
        virtual COM::Result AddSerializer(const COM::ClassID &id, const COM::ClassID& serializerID) override;
        virtual COM::Result RemoveSerializer(const COM::ClassID &id) override;
        virtual COM::Result GetSerializer(const COM::ClassID &id, ISerializer** out) override;
        virtual COM::Result GetSerializerWithID(const COM::ClassID &id, ISerializer **out) override;

    private:
        Core::HashMap<COM::ClassID, ISerializer*> mapped;
        Core::HashMap<COM::ClassID, ISerializer*> serializers;
    };
}