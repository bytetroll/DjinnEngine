//
// Created by Dev on 10/19/2017.
//
#pragma once

#include "ISerializerHost.h"

namespace Djinn::Serialization {
    class ISerializerRegistry;

    class DJINN_EXPORT_PLUGINS_SERIALIZATION SerializerHost : public ISerializerHost {
    public:
        DJINN_COM_CLASS();

        SerializerHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize the worldHost
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual COM::Result Serialize(Archive& archive, COM::IUnknown *object) override;
        virtual COM::Result Deserialize(Archive &archive, COM::IUnknown **out) override;
        virtual COM::Result Deserialize(Archive &archive, COM::IUnknown *object) override;

    private:
        ISerializerRegistry* serializerRegistry;
    };
}