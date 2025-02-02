//
// Created by Dev on 10/18/2017.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Core {
    class IStream;
}

namespace Djinn::Data {
    class IAssetConfiguration;
    class IAsset;

    class IAssetCompiler : public COM::TUnknown<IAssetCompiler> {
    public:
        DJINN_COM_INTERFACE();

        IAssetCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Compile an asset from a stream
        /// \param stream
        /// \param config
        /// \param out
        /// \return
        virtual COM::Result Compile(Core::IStream* stream, IAssetConfiguration* config, IAsset** out) = 0;
    };
}