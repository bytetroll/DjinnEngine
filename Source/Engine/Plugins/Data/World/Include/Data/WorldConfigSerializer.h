//
// Created by Dev on 1/28/2018.
//
#pragma once

#include <Serialization/ISerializer.h>

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_WORLD WorldConfigSerializer : public Serialization::ISerializer {
    public:
        DJINN_COM_CLASS();

        WorldConfigSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISerializer(registry, outer, host) {

        }

        /// Overrides
        virtual COM::Result Serialize(Serialization::Archive &archive, COM::IUnknown *object) override;
        virtual COM::Result Deserialize(Serialization::Archive &archive, COM::IUnknown **object) override;
        virtual COM::Result Deserialize(Serialization::Archive &archive, COM::IUnknown *object) override;
    };
}