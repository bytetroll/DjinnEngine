//
// Created by Dev on 9/4/2017.
//
#pragma once

#include <Serialization/ISerializer.h>

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_SHADER ShaderSerializer : public Serialization::ISerializer {
    public:
        DJINN_COM_CLASS();

        // ...
        ShaderSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        virtual COM::Result Serialize(Serialization::Archive &archive, COM::IUnknown *object) override;
        virtual COM::Result Deserialize(Serialization::Archive &archive, COM::IUnknown **object) override;
        virtual COM::Result Deserialize(Serialization::Archive &archive, COM::IUnknown *object) override;
            };
}