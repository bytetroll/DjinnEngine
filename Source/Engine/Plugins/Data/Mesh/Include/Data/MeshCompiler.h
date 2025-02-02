//
// Created by Dev on 10/23/2017.
//
#pragma once

#include "IMeshCompiler.h"
#include "MeshSourceFormat.h"

namespace Djinn::Base {
    class IFileSystem;
}

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_MESH MeshCompiler : public IMeshCompiler {
    public:
        DJINN_COM_CLASS();

        MeshCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this compiler
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual COM::Result Compile(Core::IStream *stream, IAssetConfiguration *config, IAsset **out) override;

    private:
        Base::IFileSystem* fs;
    };
}