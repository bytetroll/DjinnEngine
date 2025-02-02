//
// Created by Dev on 11/29/2017.
//
#pragma once

#include <Data/IAsset.h>
#include <GHA/ShaderBlob.h>

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_SHADER ShaderAsset : public IAsset {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        ShaderAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides

        /// Get shader blob by stage
        /// \param stage
        /// \return
        GHA::ShaderBlob* GetBlob(GHA::Stage stage) {
            return shaderBlobs[stage];
        }

        // Blobs
        Core::HashMap<GHA::Stage, GHA::ShaderBlob*> shaderBlobs;
    };
}