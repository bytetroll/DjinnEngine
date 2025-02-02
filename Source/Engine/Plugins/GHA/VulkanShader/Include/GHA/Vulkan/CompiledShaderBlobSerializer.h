//
// Created by Dev on 1/31/2018.
//
#pragma once

#include <Serialization/ISerializer.h>

namespace Djinn::GHA::Vulkan {
    class CompiledShaderBlobSerializer : public Serialization::ISerializer {
    public:
        DJINN_COM_CLASS();

        CompiledShaderBlobSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
                virtual COM::Result Serialize(Serialization::Archive &archive, COM::IUnknown *object) override;
        virtual COM::Result Deserialize(Serialization::Archive &archive, COM::IUnknown **object) override;
        virtual COM::Result Deserialize(Serialization::Archive &archive, COM::IUnknown *object) override;
    };
}