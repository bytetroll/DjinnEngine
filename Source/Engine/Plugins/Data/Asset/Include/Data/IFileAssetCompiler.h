//
// Created by Dev on 10/18/2017.
//
#pragma once

#include <COM/IUnknown.h>
#include <Base/Paths.h>
#include <Core/String.h>

namespace Djinn::Data {
    class IAsset;
    class IAssetConfiguration;

    class IFileAssetCompiler : public COM::TUnknown<IFileAssetCompiler> {
    public:
        DJINN_COM_INTERFACE();

        IFileAssetCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Compile asset from a file and automatically determine the configuration
        /// \param path
        /// \param config
        /// \param out
        /// \return
        virtual COM::Result Compile(const Base::PathID &path, const Core::String &absolute, const Core::String &extension, IAsset **out) = 0;
    };
}