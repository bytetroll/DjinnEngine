//
// Created by Dev on 3/16/2018.
//
#pragma once

#include <Data/IFileAssetCompiler.h>

namespace Djinn::Data {
    class IAssetHost;

    class DJINN_EXPORT_PLUGINS_DATA_TEXTURE TextureFileCompiler : public IFileAssetCompiler {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        TextureFileCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this compiler
        /// \param host
        /// \return
        COM::Result Initialize(IAssetHost* host);

        /// Overrides
        virtual COM::Result Compile(const Base::PathID &path, const Core::String &absolute, const Core::String &extension, IAsset **out) override;

    private:
        IAssetHost *host = nullptr;
    };
}