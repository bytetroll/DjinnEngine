//
// Created by Dev on 10/23/2017.
//
#pragma once

#include <Data/IMaterialAsset.h>
#include <Core/HashMap.h>

namespace Djinn::Data {
    class ShaderAsset;

    class DJINN_EXPORT_PLUGINS_DATA_MATERIAL BaseMaterialAsset : public IMaterialAsset {
    public:
        DJINN_COM_CLASS();

        BaseMaterialAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        virtual COM::Result GetShader(const MaterialQuery &query, ShaderAsset **out) override;

        Core::HashMap<UInt64, ShaderAsset*> shaders;
    };
}